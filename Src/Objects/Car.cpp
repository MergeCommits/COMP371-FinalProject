#include <cmath>

#include "Car.h"
#include "RectCollider.h"
#include "../Primitives/Cube.h"
#include "../Primitives/Wheel.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Texture.h"
#include "../Math/MathUtil.h"

std::vector<Car*> Car::allCars;

Car::Car(Shader* shd, Shader* colliderShd) {
    allCars.push_back(this);

    renderingMode = GL_FILL;

    collider = new RectCollider(
                            Vector2f(-2.f, 4.5f),
                            Vector2f(2.f, 4.5f),
                            Vector2f(-2.f, -3.75f),
                            Vector2f(2.f, -3.75f),
                            colliderShd
                            );

    Cube* bottom = new Cube(shd);
    bottom->setScale(4.f, 0.5f, 6.f);
    bottom->setPosition(0.f, 0.5f, 0.f);
    bottom->color = Vector4f(0.25f, 0.25f, 0.25f, 1.f);
    Cube* roof = new Cube(shd);
    roof->setScale(4.f, 0.5f, 6.f);
    roof->setPosition(0.f, 3.5f, 0.f);
    roof->color = Vector4f(0.25f, 0.25f, 1.f, 1.f);

    Cube* front = new Cube(shd);
    front->setScale(3.f, 1.f, 1.5f);
    front->setPosition(0.f, 1.f, 3.75f);
    Cube* back = new Cube(shd);
    back->setScale(3.f, 1.f, 0.75f);
    back->setPosition(0.f, 1.f, -3.375f);

    // Sides of the car.
    Cube* leftWall = new Cube(shd);
    leftWall->setScale(0.5f, 1.f, 6.f);
    leftWall->setPosition(-1.75f, 1.f, 0.f);
    Cube* rightWall = new Cube(shd);
    rightWall->setScale(0.5f, 1.f, 6.f);
    rightWall->setPosition(1.75f, 1.f, 0.f);
    Cube* frontWall = new Cube(shd);
    frontWall->setScale(3.f, 1.f, 0.5f);
    frontWall->setPosition(0.f, 1.f, 2.75f);
    Cube* backWall = new Cube(shd);
    backWall->setScale(3.f, 1.f, 0.5f);
    backWall->setPosition(0.f, 1.f, -2.75f);

    // Supports for the roof.
    Cube* topLeftPillar = new Cube(shd);
    topLeftPillar->setScale(0.5f, 1.5f, 0.5f);
    topLeftPillar->setPosition(-1.75f, 2.f, 2.75f);
    Cube* topRightPillar = new Cube(shd);
    topRightPillar->setScale(0.5f, 1.5f, 0.5f);
    topRightPillar->setPosition(1.75f, 2.f, 2.75f);
    Cube* botLeftPillar = new Cube(shd);
    botLeftPillar->setScale(0.5f, 1.5f, 0.5f);
    botLeftPillar->setPosition(-1.75f, 2.f, -2.75f);
    Cube* botRightPillar = new Cube(shd);
    botRightPillar->setScale(0.5f, 1.5f, 0.5f);
    botRightPillar->setPosition(1.75f, 2.f, -2.75f);

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
    delete collider;

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

Vector3f Car::getPosition() const {
    return position;
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

float Car::getRotationY() const {
    return rotation.y;
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

Vector2f Car::getDirectionVector(RectCollider::CollisionDir dir) {
    float sinAngle = std::sin(rotation.y);
    float cosAngle = std::cos(rotation.y);

    Vector2f retval;
    switch (dir) {
        case RectCollider::CollisionDir::Top: {
            retval = Vector2f(sinAngle, cosAngle);
        } break;

        case RectCollider::CollisionDir::Bottom: {
            retval = Vector2f(-sinAngle, -cosAngle);
        } break;

        case RectCollider::CollisionDir::Left: {
            retval = Vector2f(-cosAngle, sinAngle);
        } break;

        case RectCollider::CollisionDir::Right: {
            retval = Vector2f(cosAngle, -sinAngle);
        } break;
            
        default: {
            retval = Vector2f::zero;
        } break;
    }

    return retval;
}

void Car::update(Car::WalkInput input, float timestep) {
    acceleration = Vector2f::zero;

    if (input != WalkInput::None) {
        updateTireRotation(input, TURN_SPEED * timestep);
        updateAcceleration(input, INPUT_ACCELERATION * timestep);
    }

    updateVelocity(input, timestep);
    deltaPositionXZ = velocity;

    int collidedCarIndex = -1;
    RectCollider::CollisionDir collDir;
    if (deltaPositionCausesCollision(collidedCarIndex, collDir)) {
        float SPEED_MULTIPLIER = 10.f;
        Car* collidedCar = allCars[collidedCarIndex];
        bool pushOtherCar = (std::rand() % 2) == 1;
        if (pushOtherCar) {
            collidedCar->velocity = velocity.multiply(SPEED_MULTIPLIER);
        }
        velocity = collidedCar->getDirectionVector(collDir).multiply(SPEED_MULTIPLIER);

        deltaPositionXZ = Vector2f::zero;
        deltaRotationY = 0.f;
    }

    updatePosition(input);

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

void Car::updateVelocity(Car::WalkInput input, float timestep) {
    velocity = velocity.add(acceleration);

    if (input == WalkInput::None) {
        float timestepFriction = FRICTION * timestep;

        if (!MathUtil::eqFloats(velocity.x, 0.f) || !MathUtil::eqFloats(velocity.y, 0.f)) {
            float velocityMagnitude = velocity.length();
            float reducedLength = MathUtil::maxFloat(velocityMagnitude - timestepFriction, 0.f);
            velocity = velocity.multiply(reducedLength / velocityMagnitude);
        }
    }

    if (!MathUtil::eqFloats(velocity.x, 0.f) || !MathUtil::eqFloats(velocity.y, 0.f)) {
        float velocityMagnitude = velocity.length();
        if (velocityMagnitude > TERMINAL_VELOCITY || velocityMagnitude < -TERMINAL_VELOCITY) {
            float reducedLength = TERMINAL_VELOCITY;
            velocity = velocity.multiply(reducedLength / velocityMagnitude);
        }
    }
}

void Car::updatePosition(WalkInput input) {
    addPositionXZ(deltaPositionXZ);
    addRotationY(deltaRotationY);

    float magnitude = 0.f;
    if ((input & WalkInput::Forward) != WalkInput::None) {
        magnitude = deltaPositionXZ.length();
    } else if ((input & WalkInput::Backward) != WalkInput::None) {
        magnitude = -deltaPositionXZ.length();
    }

    if (!MathUtil::eqFloats(magnitude, 0.f)) {
        for (int i = 0; i < 4; i++) {
            wheels[i]->addRotationZ(magnitude);
        }
    }

    rotationMatrix = Matrix4x4f::rotate(rotation, position);
    Matrix4x4f scaleMatrix = Matrix4x4f::scale(scale, position);

    Matrix4x4f worldMatrix = scaleMatrix.product(rotationMatrix);

    for (int i = 0; i < (int)parts.size(); i++) {
        parts[i]->update(worldMatrix);
    }
    for (int i = 0; i < 4; i++) {
        wheels[i]->update(worldMatrix);
    }
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

bool Car::deltaPositionCausesCollision(int& collidedCar, RectCollider::CollisionDir& dir) {
    Vector3f newPosition = position.add(Vector3f(deltaPositionXZ.x, 0.f, deltaPositionXZ.y));
    Vector3f newRotation = rotation.add(Vector3f(0.f, deltaRotationY, 0.f));
    collider->update(Matrix4x4f::constructWorldMat(newPosition, Vector3f::one, newRotation));

    bool didCollide = false;
    for (int i = 0; i < (int)allCars.size(); i++) {
        if (allCars[i] == this) { continue; }

        if (collider->collides(allCars[i]->collider, dir)) {
            collidedCar = i;
            didCollide = true;
            break;
        }
    }

    if (didCollide) {
        // Restore original collider.
        collider->update(Matrix4x4f::constructWorldMat(position, Vector3f::one, rotation));
    }

    return didCollide;
}

Matrix4x4f Car::getRotationMatrix() const {
    return rotationMatrix;
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

    collider->render();
}

const Car::WalkInput operator&(const Car::WalkInput& a, const Car::WalkInput& b) {
    return (Car::WalkInput)((int)a & (int)b);
}

const Car::WalkInput operator|(const Car::WalkInput& a, const Car::WalkInput& b) {
    return (Car::WalkInput)((int)a | (int)b);
}
