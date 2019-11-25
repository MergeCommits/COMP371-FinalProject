#ifndef WHEEL_H_INCLUDED
#define WHEEL_H_INCLUDED

#include "../Graphics/Shader.h"

class Mesh;

class Wheel {
private:
    Vector3f position;
    Vector3f scale;
    Vector3f rotation;
    // Lateral rotation of wheels.
    float tireRotation;

    Mesh* mesh;
    Matrix4x4f worldMatrix;
    Shader::Uniform* worldMatrixUniform;
    Shader::Uniform* colorUniform;
    
public:
    Vector4f color;
    
    Wheel(Shader* shd);

    void setPosition(float x, float y, float z);
    void addPositionXZ(const Vector2f& vect);
    void addRotationX(float bruh);
    void addRotationZ(float bruh);
    void setTireRotation(float bruh);

    void setShader(Shader* shd);
    
    void update(const Matrix4x4f& originWorldMatrix);
    void render();
};

#endif // WHEEL_H_INCLUDED
