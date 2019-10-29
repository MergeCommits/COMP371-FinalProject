#include <stdexcept>

#include "Quad.h"
#include "Mesh.h"

Quad::Quad(Shader* shd) {
    GLuint err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Uncaught exception - Quad(Shader* shd).");
    }
    
    mesh = new Mesh(shd);
    std::vector<float> verts;
    for (int i = 0; i < 16; i++) {
        verts.push_back(quadVertices[i]);
    }
    std::vector<int> prims = {
        2, 0, 1,
        2, 1, 3
    };
    
    mesh->setGeometry(verts, prims);
}

void Quad::render() {
    mesh->render();
}
