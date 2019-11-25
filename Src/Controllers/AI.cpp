#include <cmath>

#include "AI.h"
#include "../Objects/Car.h"

AI::AI(Shader* shd, Shader* colliderShd) {
    car = new Car(shd, colliderShd);
    stepsUntilNewTask = 0;
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

void AI::update(float timestep) {
    Car::WalkInput input = Car::WalkInput::Forward;
    
    float MIN_DISTANCE_TO_ORIGIN_BEFORE_RANDOM_MOVEMENT_SQUARED = 20.f * 20.f;
    float MIN_ANGLE_TO_REACH_ORIGIN = 0.1f;
    float distanceToOriginSquared = car->getPosition().lengthSquared();
    Vector2f vectorFromOrigin = Vector2f(car->getPosition().x, car->getPosition().z);
    
    float sinAngle = std::sin(car->getRotationY());
    float cosAngle = std::cos(car->getRotationY());
    float differenceBetweenOriginAndCar = Vector2f(sinAngle, cosAngle).normalize().dotProduct(vectorFromOrigin.normalize());
    
    if (distanceToOriginSquared > MIN_DISTANCE_TO_ORIGIN_BEFORE_RANDOM_MOVEMENT_SQUARED) {
        // Far away -> Try to get closer to origin.
        if (differenceBetweenOriginAndCar > (-1.f + MIN_ANGLE_TO_REACH_ORIGIN)) {
            // TODO: Figure out optimal turn direction.
            input = input | Car::WalkInput::Left;
        }
    } else {
        // Close -> Start random movement.
        
    }
    
    car->update(input, timestep);
}

void AI::render() const {
    car->render();
}
