#include <cmath>
#include <iostream>

#include "Car.h"
#include "Cube.h"
#include "Wheel.h"
#include "Shader.h"
#include "Texture.h"
#include "../Math/MathUtil.h"

std::vector<const Car*> Car::allCars;

Car::Car(Shader* shd) {
    allCars.push_back(this);
    
    renderingMode = GL_FILL;

    collider = RectCollider(
                            Vector2f(-0.5f, 0.5f),
                            Vector2f(0.5f, 0.5f),
                            Vector2f(-0.5f, -0.5f),
                            Vector2f(0.5f, -0.5f)
                            );
    
    colliderScale = Vector2f(4.f, 8.4f);
    
    Cube* bottom = new Cube(shd);
    bottom->setScale(4.f, 0.5f, 6.f);
    bottom->setPosition(0.f, 0.5f, 0.f);
    bottom->color = Vector4f(0.25f, 0.25f, 0.25f, 1.f);
    Cube* roof = new Cube(shd);
    roof->setScale(4.f, 0.5f, 6.f);
    roof->setPosition(0.f, 4.25f, 0.f);
    roof->color = Vector4f(0.25f, 0.25f, 1.f, 1.f);

    Cube* front = new Cube(shd);
    front->setScale(3.f, 1.25f, 1.5f);
    front->setPosition(0.f, 1.f, 3.75f);
    Cube* back = new Cube(shd);
    back->setScale(3.f, 1.25f, 1.5f);
    back->setPosition(0.f, 1.f, -3.375f);

    // Sides of the car.
    Cube* leftWall = new Cube(shd);
    leftWall->setScale(0.5f, 1.25f, 6.f);
    leftWall->setPosition(-1.75f, 1.f, 0.f);
    Cube* rightWall = new Cube(shd);
    rightWall->setScale(0.5f, 1.25f, 6.f);
    rightWall->setPosition(1.75f, 1.f, 0.f);
    Cube* frontWall = new Cube(shd);
    frontWall->setScale(3.f, 1.25f, 0.5f);
    frontWall->setPosition(0.f, 1.f, 2.75f);
    Cube* backWall = new Cube(shd);
    backWall->setScale(3.f, 1.25f, 0.5f);
    backWall->setPosition(0.f, 1.f, -2.75f);

    // Supports for the roof.
    Cube* topLeftPillar = new Cube(shd);
    topLeftPillar->setScale(0.5f, 2.f, 0.5f);
    topLeftPillar->setPosition(-1.75f, 2.25f, 2.75f);
    Cube* topRightPillar = new Cube(shd);
    topRightPillar->setScale(0.5f, 2.f, 0.5f);
    topRightPillar->setPosition(1.75f, 2.25f, 2.75f);
    Cube* botLeftPillar = new Cube(shd);
    botLeftPillar->setScale(0.5f, 2.f, 0.5f);
    botLeftPillar->setPosition(-1.75f, 2.25f, -2.75f);
    Cube* botRightPillar = new Cube(shd);
    botRightPillar->setScale(0.5f, 2.f, 0.5f);
    botRightPillar->setPosition(1.75f, 2.25f, -2.75f);
    
    wheels[0] = new Wheel(shd);
    wheels[0]->setPosition(2.f, 0.f, 2.5f);
    wheels[1] = new Wheel(shd);
    wheels[1]->setPosition(2.f, 0.f, -2.5f);
    wheels[2] = new Wheel(shd);
    wheels[2]->setPosition(-2.5f, 0.f, 2.25f);
    wheels[3] = new Wheel(shd);
    wheels[3]->setPosition(-2.5f, 0.f, -2.25f);
    
    for (int i = 0; i < 4; i++) {
        wheels[i]->color = Vector4f(0.2f, 0.2f, 0.2f, 1.f);
    }

    parts.push_back(bottom);
    parts.push_back(roof);
    parts.push_back(front);
    parts.push_back(back);
    parts.push_back(leftWall);
    parts.push_back(rightWall);
    parts.push_back(frontWall);
    parts.push_back(backWall);
    parts.push_back(topLeftPillar);
    parts.push_back(topRightPillar);
    parts.push_back(botLeftPillar);
    parts.push_back(botRightPillar);
    
    metalTexture = new Texture("Textures/metal.jpg");
    tireTexture = new Texture("Textures/tire.png");
    tireRotation = 0.f;
    deltaRotationY = 0.f;
    scale = Vector3f::one;
}

Car::~Car() {
    for (int i = 0; i < (int)parts.size(); i++) {
        delete parts[i];
    }
    
    for (int i = 0; i < 4; i++) {
        delete wheels[i];
    }
    delete metalTexture;
    delete tireTexture;
    
    for (int i = 0; i < (int)allCars.size(); i++) {
        if (allCars[i] == this) {
            allCars.erase(allCars.begin() + i);
            break;
        }
    }
}

void Car::addPositionXZ(const Vector2f& vect) {
    position.x += vect.x;
    position.z += vect.y;
    for (int i = 0; i < (int)parts.size(); i++) {
        parts[i]->addPositionXZ(vect);
    }
    for (int i = 0; i < 4; i++) {
        wheels[i]->addPositionXZ(vect);
    }
}

void Car::addScale(float sca) {
    scale = scale.add(Vector3f(sca, sca, sca));
}

void Car::addRotationX(float bruh) {
    rotation.x += bruh;
    for (int i = 0; i < (int)parts.size(); i++) {
        parts[i]->addRotationX(bruh);
    }
    for (int i = 0; i < 4; i++) {
        wheels[i]->addRotationX(bruh);
    }
}

void Car::addRotationY(float bruh) {
    rotation.y += bruh;
}

void Car::addRotationZ(float bruh) {
    rotation.z += bruh;
    for (int i = 0; i < (int)parts.size(); i++) {
        parts[i]->addRotationZ(bruh);
    }
    for (int i = 0; i < 4; i++) {
        wheels[i]->addRotationZ(bruh);
    }
}

void Car::setRenderingMode(GLenum mode) {
    renderingMode = mode;
}

void Car::update(Car::WalkInput input, float timestep) {
    acceleration = Vector2f::zero;
    
    if (input != WalkInput::None) {
        updateTireRotation(input, TURN_SPEED * timestep);
        updateAcceleration(input, INPUT_ACCELERATION * timestep);
    }
    
    updateVelocity(timestep);
    deltaPositionXZ = velocity;
    
    if (!deltaPositionCausesCollision()) {
        addPositionXZ(deltaPositionXZ);
        for (int i = 0; i < 4; i++) {
            wheels[i]->addRotationZ(-deltaPositionXZ.length());
        }
        addRotationY(deltaRotationY);
        
        Matrix4x4f rotationMatrix = Matrix4x4f::rotate(rotation, position);
        Matrix4x4f scaleMatrix = Matrix4x4f::scale(scale, position);
        
        Matrix4x4f worldMatrix = scaleMatrix.product(rotationMatrix);
        
        for (int i = 0; i < (int)parts.size(); i++) {
            parts[i]->update(position, worldMatrix);
        }
        for (int i = 0; i < 4; i++) {
            wheels[i]->update(worldMatrix);
        }
    }
    
    deltaPositionXZ = Vector2f::zero;
    deltaRotationY = 0.f;
}

void Car::updateAcceleration(Car::WalkInput input, float speed) {
    float sinAngle = std::sin(rotation.y);
    float cosAngle = std::cos(rotation.y);
    
    Vector2f targetDir = Vector2f::zero;
    if ((input & WalkInput::Forward) != WalkInput::None) {
        targetDir = targetDir.add(Vector2f(sinAngle,cosAngle));
    }
    if ((input & WalkInput::Backward) != WalkInput::None) {
        targetDir = targetDir.add(Vector2f(-sinAngle,-cosAngle));
    }
    
    if (targetDir.lengthSquared() < 0.01f) { return; }
    
    if ((input & WalkInput::Forward) != WalkInput::None) {
        deltaRotationY = tireRotation * 0.05f;
    } else if ((input & WalkInput::Backward) != WalkInput::None) {
        deltaRotationY = tireRotation * -0.05f;
    }
    
    acceleration = targetDir.normalize().multiply(speed);
}

void Car::updateVelocity(float timestep) {
    // Apply acceleration and friction to velocity.
    velocity = velocity.add(acceleration);
    
    float timestepFriction = FRICTION * timestep;
    
    if (!MathUtil::eqFloats(velocity.x, 0.f) || !MathUtil::eqFloats(velocity.y, 0.f)) {
        float velocityMagnitude = velocity.length();
        float reducedLength = MathUtil::maxFloat(velocityMagnitude - timestepFriction, 0.f);
        velocity = velocity.multiply(reducedLength / velocityMagnitude);
    }
    
    if (!MathUtil::eqFloats(velocity.x, 0.f) || !MathUtil::eqFloats(velocity.y, 0.f)) {
        float velocityMagnitude = velocity.length();
        if (velocityMagnitude > TERMINAL_VELOCITY || velocityMagnitude < -TERMINAL_VELOCITY) {
            float reducedLength = TERMINAL_VELOCITY;
            velocity = velocity.multiply(reducedLength / velocityMagnitude);
        }
    }
    
    // Cap velocity as its terminal.
    if (velocity.x > TERMINAL_VELOCITY) { velocity.x = TERMINAL_VELOCITY; }
    else if (velocity.x < -TERMINAL_VELOCITY) { velocity.x = -TERMINAL_VELOCITY; }
    
    if (velocity.y > TERMINAL_VELOCITY) { velocity.y = TERMINAL_VELOCITY; }
    else if (velocity.y < -TERMINAL_VELOCITY) { velocity.y = -TERMINAL_VELOCITY; }
    
    
    std::cout << "VELOCITY: " << velocity.x << ", " << velocity.y << std::endl;
}

void Car::updateTireRotation(WalkInput input, float speed) {
    float deltaTireRotation = 0.f;
    if ((input & WalkInput::Left) != WalkInput::None) {
        deltaTireRotation -= speed;
    }
    if ((input & WalkInput::Right) != WalkInput::None) {
        deltaTireRotation += speed;
    }
    
    float tireReturnToCenterSpeed = speed * 0.75f;
    if (MathUtil::eqFloats(deltaTireRotation, 0.f)) {
        if (tireRotation > 0.f) {
            tireRotation = MathUtil::clampFloat(tireRotation - tireReturnToCenterSpeed, 0.f, tireRotation);
        } else if (tireRotation < 0.f) {
            tireRotation = MathUtil::clampFloat(tireRotation + tireReturnToCenterSpeed, tireRotation, 0.f);
        }
    } else {
        tireRotation += deltaTireRotation;
    }
    
    tireRotation = MathUtil::clampFloat(tireRotation, -1.f, 1.f);
    for (int i = 0; i < 4; i++) {
        wheels[i]->setTireRotation(tireRotation);
    }
}

bool Car::deltaPositionCausesCollision() {
    Vector3f newPosition = position.add(Vector3f(deltaPositionXZ.x, 0.f, deltaPositionXZ.y));
    Vector3f newRotation = rotation.add(Vector3f(0.f, deltaRotationY, 0.f));
    collider.update(Matrix4x4f::constructWorldMat(newPosition, Vector3f(colliderScale.x, 1.f, colliderScale.y), newRotation));
    
    for (int i = 0; i < (int)allCars.size(); i++) {
        if (allCars[i] == this) { continue; }
        
        RectCollider::CollisionDir dir;
        if (collider.collides(allCars[i]->collider, dir)) {
            return true;
        }
    }
    
    return false;
}

void Car::setShader(Shader* shd) {
    for (int i = 0; i < (int)parts.size(); i++) {
        parts[i]->setShader(shd);
    }
    for (int i = 0; i < 4; i++) {
        wheels[i]->setShader(shd);
    }
}

void Car::render() {
    glPolygonMode(GL_FRONT_AND_BACK, renderingMode);
    metalTexture->activate(1);
    for (int i = 0; i < (int)parts.size(); i++) {
        parts[i]->render();
    }
    tireTexture->activate(1);
    for (int i = 0; i < 4; i++) {
        wheels[i]->render();
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

const Car::WalkInput operator&(const Car::WalkInput& a, const Car::WalkInput& b) {
    return (Car::WalkInput)((int)a & (int)b);
}

const Car::WalkInput operator|(const Car::WalkInput& a, const Car::WalkInput& b) {
    return (Car::WalkInput)((int)a | (int)b);
}
