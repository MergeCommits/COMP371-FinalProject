#ifndef TRIANGLE_H_INCLUDED
#define TRIANGLE_H_INCLUDED

#include <vector>

#include "Shader.h"

class Mesh;

class Triangle {
private:
    const float vertices[9] = {
        0.f, 0.5f, 0.f,    // Top.
        -0.5f, -0.5f, 0.f, // Left.
        0.5f, -0.5f, 0.f   // Right.
    };
    
    Mesh* mesh;
    
    Shader::Uniform* worldMat;
    Shader::Uniform* colorUniform;

public:
    Triangle(Shader* shd);
    
    void render();
};

#endif // TRIANGLE_H_INCLUDED
