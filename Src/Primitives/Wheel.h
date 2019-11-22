#ifndef WHEEL_H_INCLUDED
#define WHEEL_H_INCLUDED

#include "Shader.h"

class Mesh;

class Wheel {
private:
    Vector3f position;
    // Scales relative to an arbitrary origin.
    Vector3f scaleOrigin;
    Vector3f scale;
    // Rotations relative to an arbitrary origin.
    Vector3f rotationOrigin;
    Vector3f rotation;
    // Lateral rotation of wheels.
    float tireRotation;

    Mesh* mesh;
    Shader::Uniform* worldMat;
    Shader::Uniform* colorUniform;
    
public:
    Vector4f color;
    
    Wheel(Shader* shd);

    void setPosition(float x, float y, float z);
    void addPositionXZ(const Vector2f& vect);
    void addScaleOrigin(float sca);
    void addRotationX(float bruh);
    void addRotationOriginY(float bruh);
    void addRotationZ(float bruh);
    void setTireRotation(float bruh);

    void setShader(Shader* shd);

    void render();
    void render(const Vector3f& origin);
};

#endif // WHEEL_H_INCLUDED
