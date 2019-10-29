#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include <GL/glew.h>
#include <vector>

class Shader;

class Mesh {
private:
    GLuint vertexArrayObject = 0;
    GLuint vertexBufferObject = 0;
    GLuint primitiveBuffer = 0;
    
    std::vector<float> vertexData;
    std::vector<int> primData;
    
    Shader* shader;
    GLenum mode;

    bool needsUpload;
    
    void uploadData();

public:
    Mesh(Shader* shd);
    ~Mesh();

    void setGeometry(std::vector<float>& vertices, std::vector<int>& primitives, GLenum mod = GL_TRIANGLES);
    void setShader(Shader* shd);
    void render();
};

#endif // MESH_H_INCLUDED
