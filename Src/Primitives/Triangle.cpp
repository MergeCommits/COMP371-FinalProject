#include <GL/glew.h>
#include <stdexcept>
#include <stdexcept>

#include "Triangle.h"
#include "Mesh.h"

Triangle::Triangle(Shader* shd) {
    GLuint err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Uncaught exception - Triangle(Shader* shd).");
    }
    
    mesh = new Mesh(shd);
    std::vector<float> verts;
    verts.push_back(vertices[0]);
    verts.push_back(vertices[1]);
    verts.push_back(vertices[2]);
    verts.push_back(vertices[3]);
    verts.push_back(vertices[4]);
    verts.push_back(vertices[5]);
    verts.push_back(vertices[6]);
    verts.push_back(vertices[7]);
    verts.push_back(vertices[8]);
    std::vector<int> prims = {
        0, 1, 2
    };
    
    mesh->setGeometry(verts, prims);
    
    worldMat = shd->getMat4Uniform("modelMatrix");
    colorUniform = shd->getVec4fUniform("fsColor");
}

void Triangle::render() {
    Matrix4x4f mat = Matrix4x4f::constructWorldMat(Vector3f::zero, Vector3f::one, Vector3f::zero);
    worldMat->setValue(mat);
    colorUniform->setValue(Vector4f(1.f, 0.f, 0.f, 1.f));
    
    mesh->render();
}
