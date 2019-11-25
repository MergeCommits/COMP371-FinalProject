#ifndef AXIS_H_INCLUDED
#define AXIS_H_INCLUDED

#include "../Graphics/Shader.h"

class Mesh;

class Axis {
private:
    const float vertices[12] = {
        // Position.    // Normal.
        0.f, 0.f, 0.f,  0.f, 1.f, 0.f,
        0.f, 0.f, -1.f, 0.f, 1.f, 0.f
    };

    Mesh* mesh;
    Shader::Uniform* worldMat;
    Shader::Uniform* colorUniform;

public:
    Vector3f rotation;
    Vector4f color;

    Axis(Shader* shd);
    
    void render();
};

#endif // AXIS_H_INCLUDED
