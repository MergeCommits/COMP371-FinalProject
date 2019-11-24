#ifndef CUBE_H_INCLUDED
#define CUBE_H_INCLUDED

#include "Shader.h"

class Mesh;

class Cube {
private:
    const float vertices[288] = {
        // Positions         // Normals          // UV Coords
        // back face
        -0.5f, 0.0f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
         0.5f, 1.0f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
         0.5f, 0.0f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
         0.5f, 1.0f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
        -0.5f, 0.0f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -0.5f, 1.0f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
        // front face
        -0.5f, 0.0f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
         0.5f, 0.0f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
         0.5f, 1.0f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
         0.5f, 1.0f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
        -0.5f, 1.0f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
        -0.5f, 0.0f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -0.5f, 1.0f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        -0.5f, 1.0f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
        -0.5f, 0.0f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -0.5f, 0.0f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -0.5f, 0.0f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -0.5f, 1.0f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        // right face
         0.5f, 1.0f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         0.5f, 0.0f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         0.5f, 1.0f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
         0.5f, 0.0f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         0.5f, 1.0f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         0.5f, 0.0f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
        // bottom face
        -0.5f, 0.0f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
         0.5f, 0.0f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
         0.5f, 0.0f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
         0.5f, 0.0f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
        -0.5f, 0.0f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -0.5f, 0.0f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
        // top face
        -0.5f, 1.0f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
         0.5f, 1.0f , 0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
         0.5f, 1.0f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
         0.5f, 1.0f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
        -0.5f, 1.0f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
        -0.5f, 1.0f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
    };

    Vector3f position;
    Vector3f scale;
    Vector3f rotation;

    Mesh* mesh;
    Matrix4x4f worldMatrix;
    Shader::Uniform* worldMatUniform;
    Shader::Uniform* colorUniform;

public:
    Vector4f color;

    Cube(Shader* shd);

    void setPosition(float x, float y, float z);
    void addPositionXZ(const Vector2f& vect);
    void setScale(float x, float y, float z);
    void addRotationX(float bruh);
    void addRotationY(float bruh);
    void addRotationZ(float bruh);

    void setShader(Shader* shd);
    
    void constructWorldMat();
    void constructWorldMat(const Vector3f& origin, const Matrix4x4f& originWorldMatrix);
    Matrix4x4f getWorldMatrix() const;

    void update(const Vector3f& origin, const Matrix4x4f& originWorldMatrix);
    void render() const;
};

#endif // CUBE_H_INCLUDED
