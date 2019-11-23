#include <stdexcept>

#include "Cube.h"
#include "Mesh.h"

Cube::Cube(Shader* shd) {
    GLuint err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Uncaught exception - Cube(Shader* shd).");
    }
    
    mesh = new Mesh(shd);
    setShader(shd);
    std::vector<float> verts;
    for (int i = 0; i < 288; i++) {
        verts.push_back(vertices[i]);
    }
    std::vector<int> prims = {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11,
        12, 13, 14,
        15, 16, 17,
        18, 19, 20,
        21, 22, 23,
        24, 25, 26,
        27, 28, 29,
        30, 31, 32,
        33, 34, 35
    };
    
    mesh->setGeometry(verts, prims);

    color = Vector4f(1.f, 0.f, 0.f, 1.f);
    scale = Vector3f::one;
    scaleOrigin = Vector3f::one;
}

void Cube::setPosition(float x, float y, float z) {
    position = Vector3f(x, y, z);
}

void Cube::addPositionXZ(const Vector2f& vect) {
    position.x += vect.x;
    position.z += vect.y;
}

void Cube::setScale(float x, float y, float z) {
    scale = Vector3f(x, y, z);
}

void Cube::addRotationX(float bruh) {
    rotation.x += bruh;
}

void Cube::addRotationY(float bruh) {
    rotation.y += bruh;
}

void Cube::addRotationZ(float bruh) {
    rotation.z += bruh;
}

void Cube::setShader(Shader* shd) {
    mesh->setShader(shd);
    worldMatUniform = shd->getMat4Uniform("modelMatrix");
    colorUniform = shd->getVec4fUniform("fsColor");
}

void Cube::constructWorldMat() {
    worldMatrix = Matrix4x4f::constructWorldMat(position, scale, rotation);
}

void Cube::constructWorldMat(const Vector3f& origin, const Matrix4x4f& originWorldMatrix) {
    Matrix4x4f scaleMat = Matrix4x4f::scale(scale);
    Matrix4x4f rotateMat = Matrix4x4f::rotate(rotation, Vector3f(0.f, 0.5f, 0.f));
    
    worldMatrix = scaleMat.product(Matrix4x4f::translate(position).product(rotateMat.product(originWorldMatrix)));
}

Matrix4x4f Cube::getWorldMatrix() const {
    return worldMatrix;
}

void Cube::update(const Vector3f& origin, const Matrix4x4f& originWorldMatrix) {
    constructWorldMat(origin, originWorldMatrix);
}

void Cube::render() const {
    worldMatUniform->setValue(worldMatrix);
    colorUniform->setValue(color);
    glCullFace(GL_FRONT);
    mesh->render();
    glCullFace(GL_BACK);
    
}
