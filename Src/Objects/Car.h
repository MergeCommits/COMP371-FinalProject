#ifndef CAR_H_INCLUDED
#define CAR_H_INCLUDED

#include <vector>
#include <GL/glew.h>

#include "../Math/Vector.h"

class Cube;
class Wheel;
class Shader;
class Texture;

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
    Vector3f position;
    Vector3f scale;
    Vector3f rotation;
    float tireRotation;
    GLenum renderingMode;
    
    Texture* metalTexture;
    Texture* tireTexture;
    
    std::vector<Cube*> parts;
    Wheel* wheels[4];
    
    void walk(WalkInput input, float speed);
    
public:
    Car(Shader* shd);
    ~Car();
    
    void update(WalkInput input, float speed);
    void addPositionXZ(const Vector2f& vect);
    void addScale(float sca);
    void addRotationX(float bruh);
    void addRotationY(float bruh);
    void addRotationZ(float bruh);
    void setRenderingMode(GLenum mode);
    
    void setShader(Shader* shd);
    
    void render();
};

const Car::WalkInput operator&(const Car::WalkInput& a, const Car::WalkInput& b);
const Car::WalkInput operator|(const Car::WalkInput& a, const Car::WalkInput& b);

#endif // CAR_H_INCLUDED
