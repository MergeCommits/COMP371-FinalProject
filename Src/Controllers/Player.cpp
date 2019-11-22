#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Player.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Camera.h"
#include "../Objects/Car.h"

Player::Player(Shader* shd, int camWidth, int camHeight) {
    car = new Car(shd);
    camera = new Camera(camWidth, camHeight);

	cameraFollowingCar = true;
}

Player::~Player() {
    delete camera;
    delete car;
}

Camera* Player::getCamera() const {
    return camera;
}

void Player::setCarShader(Shader* shd) {
    car->setShader(shd);
}

void Player::update(float timestep, GLFWwindow* window) {
    // Movement.
    float speed = 5.f;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        speed += 15.f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        speed = 2.f;
    }
    
    Car::WalkInput input = Car::WalkInput::None;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        input = input | Car::WalkInput::Forward;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        input = input | Car::WalkInput::Backward;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        input = input | Car::WalkInput::Left;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        input = input | Car::WalkInput::Right;
    }
    car->walk(input, timestep * speed);

	camera->update();
}

void Player::render() const {
    car->render();
}
