#include <iostream>
#include <fstream>
#include <ctime>

#define GLEW_STATIC 1
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "Timing.h"
#include "Controllers/Player.h"
#include "Graphics/Grid.h"
#include "Graphics/Quad.h"
#include "Graphics/Axis.h"
#include "Graphics/Wheel.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera.h"
#include "Graphics/Texture.h"
#include "Math/MathUtil.h"

int width = 1024;
int height = 768;

// Mouse position last frame.
float prevMouseX = 0.f;
float prevMouseY = 0.f;
// Difference between the mouse position in this frame and the previous frame.
float mouseXDiff = 0.f;
float mouseYDiff = 0.f;

// Whether to perform a depth pass for shadow mapping.
bool enableShadows = true;

// Whether to use texture colors or vertex colors.
bool enableTextures = true;

// Whether to render the depth map to a texture for debugging.
bool debugDepthMap = false;

void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void windowSizeCallback(GLFWwindow* window, int w, int h);
void updateInputs(float timestep, GLFWwindow* window, Camera* cam);

int main() {
    // Give std::rand the current time as a seed.
    std::srand((unsigned)std::time(NULL));
    
    // Initialize GLFW and OpenGL version.
    glfwInit();

#if defined(PLATFORM_OSX)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
	// On windows, we set OpenGL version to 2.1, to support more hardware.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    // Create Window and rendering context using GLFW.
    GLFWwindow* window = glfwCreateWindow(width, height, "Comp371 - Lab 01", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPos(window, width / 2.f, height / 2.f);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    prevMouseX = (float)mouseX;
    prevMouseY = (float)mouseY;

    // Initialize GLEW.
    glewExperimental = true; // Needed for core profile.
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    // Fixed time steps.
    Timing* timing = new Timing(60);
    
    // Cameras.
    Camera* cam = new Camera(width, height);
    cam->setPosition(Vector3f(0.f, 5.f, -20.f));
    
    bool directionalShadow = false;
    float nearZ = directionalShadow ? 1.f : 20.f;
    Camera* light = new Camera(100, 100, MathUtil::degToRad(115.f), nearZ, 40.f, directionalShadow);
    light->setPosition(Vector3f(0.f, 30.f, 0.f));
    light->addAngle(0.f, MathUtil::PI / (directionalShadow ? -2.f : 2.f));
    
    // Shaders.
    Shader* defaultShader = new Shader("Shaders/default/");
    defaultShader->addVec3VertexInput("position");
    defaultShader->addVec3VertexInput("normal");
    cam->addShader(defaultShader);
    
    Shader* imageShader = new Shader("Shaders/Image/");
    imageShader->addVec2VertexInput("position");
    imageShader->addVec2VertexInput("uv");
    
    Shader* depthPassShader = new Shader("Shaders/DepthPass/");
    depthPassShader->addVec3VertexInput("position");
    depthPassShader->addVec3VertexInput("normal");
    depthPassShader->addVec2VertexInput("uv");
    
    Shader* shadowPassShader = new Shader("Shaders/ShadowPass/");
    shadowPassShader->addVec3VertexInput("position");
    shadowPassShader->addVec3VertexInput("normal");
    shadowPassShader->addVec2VertexInput("uv");
    cam->addShader(shadowPassShader);
    
    GLuint err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Failed to create shaders.");
    }
    
    Player* player = new Player(defaultShader);

    // Models.

    // 100x100 grid.
    Grid* grid = new Grid(depthPassShader);
    grid->scale = Vector3f(50.f, 1.f, 50.f);
    
    Quad* quad = new Quad(imageShader);
    Texture* testTex = new Texture("Textures/grass.png");
    
    Axis* xAxis = new Axis(defaultShader);
    xAxis->color = Vector4f(1.f, 0.f, 0.f, 1.f);
    xAxis->rotation = Vector3f(0.f, MathUtil::PI / 2.f, 0.f);
    Axis* yAxis = new Axis(defaultShader);
    yAxis->color = Vector4f(0.f, 0.f, 1.f, 1.f);
    yAxis->rotation = Vector3f(MathUtil::PI / -2.f, 0.f, 0.f);
    Axis* zAxis = new Axis(defaultShader);
    zAxis->color = Vector4f(0.f, 0.75f, 0.f, 1.f);
    
    // Textures.
    Texture* grass = new Texture("Textures/grass.jpg");
    
    // Shadows.
    GLuint depthMapFrameBuffer;
    glGenFramebuffers(1, &depthMapFrameBuffer);
    
    GLuint SHADOW_DIMENSIONS = 2048;

    GLuint depthMapTextureID;
    glGenTextures(1, &depthMapTextureID);
    glBindTexture(GL_TEXTURE_2D, depthMapTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, SHADOW_DIMENSIONS, SHADOW_DIMENSIONS, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFrameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTextureID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Failed to create depth map.");
    }
    
    imageShader->getIntUniform("tex0")->setValue(0);
    shadowPassShader->getIntUniform("shadowMap")->setValue(0);
    shadowPassShader->getIntUniform("diffuse")->setValue(1);

    while (!glfwWindowShouldClose(window)) {
        while (timing->tickReady()) {
            float timestep = (float)timing->getTimeStep();
            
            // Detect inputs.
            mouseXDiff = 0.f;
            mouseYDiff = 0.f;
            glfwPollEvents();
            
            updateInputs(timestep, window, cam);
            player->update(timestep, window);
            
            timing->subtractTick();
        }

        // Draw code.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // If the aspect ratio was changed then update the camera's size.
        if (!MathUtil::eqFloats(cam->getAspectRatio(), (float)width / height)) {
            cam->setXYClippings(width, height);
        }
        
        cam->update();
        light->update();
        
        // Render scene from light's point of view into a depth map.
        if (enableShadows) {
            depthPassShader->getMat4Uniform("depthViewMatrix")->setValue(light->getViewMatrix());
            depthPassShader->getMat4Uniform("depthProjectionMatrix")->setValue(light->getProjectionMatrix());

            glViewport(0, 0, SHADOW_DIMENSIONS, SHADOW_DIMENSIONS);
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFrameBuffer);
            
            glClear(GL_DEPTH_BUFFER_BIT);
            
            grid->setShader(depthPassShader);
            grid->render();
            player->setShader(depthPassShader);
            player->render();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // Reset viewport.
#if defined(PLATFORM_OSX)
			// Due to macOS retina displays the backbuffer's size is actually twice of what is passed to the window, so double it here.
			glViewport(0, 0, width * 2, height * 2);
#else
			glViewport(0, 0, width, height);
#endif
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        
        if (!debugDepthMap) {
            // Render the scene from the camera's position.
            shadowPassShader->getBoolUniform("enableShadows")->setValue(enableShadows);
            shadowPassShader->getBoolUniform("enableTextures")->setValue(enableTextures);
            shadowPassShader->getVec3fUniform("cameraPosition")->setValue(cam->getPosition());
            shadowPassShader->getVec3fUniform("lightPosition")->setValue(light->getPosition());
            shadowPassShader->getMat4Uniform("lightViewMatrix")->setValue(light->getViewMatrix());
            shadowPassShader->getMat4Uniform("lightProjectionMatrix")->setValue(light->getProjectionMatrix());
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, depthMapTextureID);
            grass->activate(1);
            grid->setShader(shadowPassShader);
            grid->render();
            player->setShader(shadowPassShader);
            player->render();

            glDisable(GL_DEPTH_TEST);
            xAxis->render();
            yAxis->render();
            zAxis->render();
            glEnable(GL_DEPTH_TEST);
        } else {
            // Render depth map to quad for debugging.
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, depthMapTextureID);

            glDisable(GL_DEPTH_TEST);
            quad->render();
            glEnable(GL_DEPTH_TEST);
        }

        glfwSwapBuffers(window);
        
        // Get elapsed seconds since last run.
        double secondsPassed = timing->getElapsedSeconds();
        timing->addSecondsToAccumulator(secondsPassed);
    }

    delete cam;
    delete light;
    delete player;
    delete grid;
    delete xAxis;
    delete yAxis;
    delete zAxis;
    delete grass;
    delete defaultShader;
    delete imageShader;
    delete depthPassShader;
    delete shadowPassShader;
    delete quad;
    delete testTex;

    // Shutdown GLFW
    glfwTerminate();

	return 0;
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        mouseXDiff = (float)xpos - prevMouseX;
        mouseYDiff = (float)ypos - prevMouseY;
        prevMouseX = (float)xpos;
        prevMouseY = (float)ypos;
        
        float sensitivity = 0.01f;
        mouseXDiff *= sensitivity;
        mouseYDiff *= sensitivity;
    }
}

void windowSizeCallback(GLFWwindow* window, int w, int h) {
    width = w;
    height = h;
}

// Used to determine whether a key was HIT, as opposed to just pressed.
int lastKeySpaceState = GLFW_RELEASE;
int lastKeyXState = GLFW_RELEASE;
int lastKeyBState = GLFW_RELEASE;
int lastKeyKState = GLFW_RELEASE;

static bool inputHit(GLFWwindow* window, int key, int& lastKeyState) {
    int prevState = lastKeyState;
    lastKeyState = glfwGetKey(window, key);
    if (prevState == GLFW_RELEASE) {
        return lastKeyState == GLFW_PRESS;
    }
    
    return false;
}

void updateInputs(float timestep, GLFWwindow* window, Camera* cam) {
    // Cursor position.
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        return;
    }
    
    // Toggle textures.
    if (inputHit(window, GLFW_KEY_X, lastKeyXState)) {
        enableTextures = !enableTextures;
    }
    
    // Toggle shadow map.
    if (inputHit(window, GLFW_KEY_B, lastKeyBState)) {
        enableShadows = !enableShadows;
    }
    
    // Toggle depth map view.
    if (inputHit(window, GLFW_KEY_K, lastKeyKState)) {
        debugDepthMap = !debugDepthMap;
    }
    
    cam->addAngle(mouseXDiff, mouseYDiff);
}
