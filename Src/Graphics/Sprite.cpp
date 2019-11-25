#include <cmath>
#include <vector>

#include "Sprite.h"
#include "Mesh.h"

Sprite::Sprite(Shader* shd) {
    GLuint err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Uncaught exception - Sprite(Shader* shd).");
    }
    
    mesh = new Mesh(shd);
    std::vector<float> verts = {
        -0.5f, 0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f,
        0.5f, 0.5f
    };
    std::vector<int> prims = {
        0, 1, 2,
        0, 3, 1
    };
    mesh->setGeometry(verts, prims);
    
    modelMatrixUniform = shd->getMat4Uniform("modelMatrix");
    scaleUniform = shd->getVec2fUniform("scale");
    rotationMatrixUniform = shd->getMat4Uniform("rotationMatrix");
    colorUniform = shd->getVec4fUniform("spriteColor");
    
    scale = Vector2f::one;
}

Sprite::~Sprite() {
    delete mesh;
}

void Sprite::setScale(float scale) {
    this->scale = scale;
}

void Sprite::setPosition(const Vector3f& pos) {
    position = pos;
}

void Sprite::addRotation(float rad) {
    rotation += rad;
}

void Sprite::setOpacity(float value) {
    color.w = value;
}

void Sprite::update() {
    // TODO: Convert to matrix functions.
    modelMatrix = Matrix4x4f::constructWorldMat(position, Vector3f::one, Vector3f::zero);
    float sinRoll = sin(rotation);
    float cosRoll = cos(rotation);
    rotationMatrix = Matrix4x4f(cosRoll,-sinRoll,0.f,0.f,
                                sinRoll,cosRoll,0.f,0.f,
                                0.f,0.f,1.f,0.f,
                                0.f,0.f,0.f,1.f);
}

void Sprite::render() const {
    modelMatrixUniform->setValue(modelMatrix);
    scaleUniform->setValue(scale);
    rotationMatrixUniform->setValue(rotationMatrix);
    colorUniform->setValue(color);

    mesh->render();
}
