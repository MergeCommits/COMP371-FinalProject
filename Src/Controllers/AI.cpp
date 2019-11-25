#include <cmath>

#include "AI.h"

AI::AI(Shader* shd, Shader* colliderShd) {
    car = new Car(shd, colliderShd);
    
    stepsUntilNewTask = 0;
    currTaskDirection = Car::WalkInput::None;
    
    brainFreeze = true;
}

AI::~AI() {
    delete car;
}

void AI::setCarShader(Shader* shd) {
    car->setShader(shd);
}

Car* AI::getCar() const {
    return car;
}

Car::WalkInput AI::getCurrentTaskDirection() {
    if (currTaskDirection == Car::WalkInput::None) {
        updateCurrentTaskDirection();
    }
    return currTaskDirection;
}

void AI::updateCurrentTaskDirection() {
    currTaskDirection = (std::rand() % 2) == 0 ? Car::WalkInput::Left : Car::WalkInput::Right;
}

void AI::toggleBrainFreeze() {
    brainFreeze = !brainFreeze;
}

void AI::updateFarInput(Car::WalkInput& input) {
    float MIN_ANGLE_TO_REACH_ORIGIN = 0.1f;
    Vector2f vectorFromOrigin = Vector2f(car->getPosition().x, car->getPosition().z);
    
    float sinAngle = std::sin(car->getRotationY());
    float cosAngle = std::cos(car->getRotationY());
    float differenceBetweenOriginAndCar = Vector2f(sinAngle, cosAngle).normalize().dotProduct(vectorFromOrigin.normalize());
    
    if (differenceBetweenOriginAndCar > (-1.f + MIN_ANGLE_TO_REACH_ORIGIN)) {
        input = input | getCurrentTaskDirection();
        
    }
}

void AI::updateNearInput(Car::WalkInput& input) {
    int STEPS_WITHOUT_NEW_TASK = 10;
    if (stepsUntilNewTask < -STEPS_WITHOUT_NEW_TASK) {
        stepsUntilNewTask = (std::rand() % 30) + 1;
        updateCurrentTaskDirection();
    }
    
    stepsUntilNewTask--;
    input = input | getCurrentTaskDirection();
}

void AI::update(float timestep) {
    if (brainFreeze) {
        car->update(Car::WalkInput::None, timestep);
        return;
    }
    
    float MIN_DISTANCE_TO_ORIGIN_BEFORE_RANDOM_MOVEMENT_SQUARED = 30.f * 30.f;
    float distanceToOriginSquared = car->getPosition().lengthSquared();
    Car::WalkInput input = Car::WalkInput::Forward;
    
    if (distanceToOriginSquared > MIN_DISTANCE_TO_ORIGIN_BEFORE_RANDOM_MOVEMENT_SQUARED) {
        updateFarInput(input);
    } else {
        updateNearInput(input);
    }
    
    car->update(input, timestep);
}

void AI::render() const {
    car->render();
}
