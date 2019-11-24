#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Player.h"
#include "../InputUtil.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Camera.h"
#include "../Objects/Car.h"

Player::Player(Shader* shd, Shader* colliderShd, int camWidth, int camHeight) {
    car = new Car(shd, colliderShd);
    camera = new Camera(camWidth, camHeight);
    camera->addAngle(MathUtil::PI, 0.f);

	cameraFollowingCar = true;
    camera->setThirdPersonPerspective(true);
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

int lastKeyCState = GLFW_RELEASE;

void Player::update(float timestep, GLFWwindow* window) {
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
    car->update(input, timestep);
    
    if (InputUtil::keyHit(window, GLFW_KEY_C, lastKeyCState)) {
        camera->setThirdPersonPerspective(!camera->isThirdPerson());
    }
    
    if (cameraFollowingCar) {
        Vector3f pos = car->getPosition();
        if (camera->isThirdPerson()) {
            pos = pos.add(Vector3f(0.f, 5.f, 0.f));
        } else {
            pos = car->getRotationMatrix().transform(pos.add(Vector3f(0.f, 2.f, 6.f)));
        }
        camera->setPosition(pos);
    }

	camera->update();
}

void Player::render() const {
    car->render();
}
