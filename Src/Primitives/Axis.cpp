#include <stdexcept>

#include "Axis.h"
#include "Mesh.h"

Axis::Axis(Shader* shd) {
    GLuint err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Uncaught exception - Axes(Shader* shd).");
    }
    
    mesh = new Mesh(shd);
    std::vector<float> verts;
    for (int i = 0; i < 12; i++) {
        verts.push_back(vertices[i]);
    }
    std::vector<int> prims = {
        0, 1
    };
    
    mesh->setGeometry(verts, prims, GL_LINES);

    worldMat = shd->getMat4Uniform("modelMatrix");
    colorUniform = shd->getVec4fUniform("fsColor");
    rotation = Vector3f::zero;
    color = Vector4f::one;
}

void Axis::render() {
//   Matrix4x4f mat = Matrix4x4f::rotate(rotation);
    Matrix4x4f mat = Matrix4x4f::constructWorldMat(Vector3f::zero, Vector3f(2.f, 2.f, 2.f), rotation);
    worldMat->setValue(mat);
    colorUniform->setValue(color);
    
    mesh->render();
}
