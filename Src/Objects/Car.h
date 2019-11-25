#ifndef CAR_H_INCLUDED
#define CAR_H_INCLUDED

#include <vector>
#include <GL/glew.h>

#include "RectCollider.h"
#include "../Math/Vector.h"
#include "../Math/Matrix.h"

class Cube;
class Wheel;
class Shader;
class Texture;
class Smoke;

class Car {
public:
    enum class WalkInput {
        None = 0x0,
        Forward = 0x1,
        Backward = 0x2,
        Left = 0x4,
        Right = 0x8
    };
    
private:
    static std::vector<Car*> allCars;
    
    const float INPUT_ACCELERATION = 1.f;
    const float TURN_SPEED = 7.f;
    const float TERMINAL_VELOCITY = 0.25f;
    const float FRICTION = 1.f;
    
    Vector3f position;
    Vector2f deltaPositionXZ;
    Vector2f velocity;
    Vector2f acceleration;
    Vector3f scale;
    Vector3f rotation;
    float deltaRotationY;
    float tireRotation;
    GLenum renderingMode;
    
    Matrix4x4f rotationMatrix;
    
    Texture* metalTexture;
    Texture* tireTexture;
    
    Shader* spriteShader;
    std::vector<Smoke*> smokeParticles;
    
    std::vector<Cube*> parts;
    Wheel* wheels[4];
    RectCollider* collider;
    
    Vector2f getDirectionVector(RectCollider::CollisionDir dir);
    
    void updateAcceleration(WalkInput input, float speed);
    void updateVelocity(Car::WalkInput input, float timestep);
    void updatePosition(WalkInput input);
    void updateTireRotation(WalkInput input, float speed);
    bool deltaPositionCausesCollision(int& collidedCar, RectCollider::CollisionDir& dir);
    
public:
    Car(Shader* shd, Shader* colliderShd, Shader* spriteShd);
    ~Car();
    
    void addPositionXZ(const Vector2f& vect);
    Vector3f getPosition() const;
    void addScale(float sca);
    void addRotationX(float bruh);
    void addRotationY(float bruh);
    float getRotationY() const;
    void addRotationZ(float bruh);
    void setRenderingMode(GLenum mode);
    
    Matrix4x4f getRotationMatrix() const;
    
    void setShader(Shader* shd);
    
    void update(WalkInput input, float timestep);
    void render();
};

const Car::WalkInput operator&(const Car::WalkInput& a, const Car::WalkInput& b);
const Car::WalkInput operator|(const Car::WalkInput& a, const Car::WalkInput& b);

#endif // CAR_H_INCLUDED
