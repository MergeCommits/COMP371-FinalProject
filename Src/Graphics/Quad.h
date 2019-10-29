#ifndef QUAD_H_INCLUDED
#define QUAD_H_INCLUDED

class Shader;
class Mesh;

class Quad {
private:
    float quadVertices[16] = {
        // positions  // texture Coords
        -1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
    };
    
    Mesh* mesh;

public:
    Quad(Shader* shd);

    void render();
};

#endif // QUAD_H_INCLUDED
