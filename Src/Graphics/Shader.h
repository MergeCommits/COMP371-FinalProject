#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <GL/glew.h>
#include <vector>

#include "../Utils/String.h"
#include "../Math/Matrix.h"

class Shader {
public:
    class Uniform {
    public:
        union Values {
            Values();
            Matrix4x4f matrixVal;
            Vector4f vec4Val;
            Vector3f vec3Val;
            int intVal;
            bool boolVal;
        };
        Values value;
        
        enum class Kind {
            Matrix,
            Vector4f,
            Vector3f,
            Integer,
            Boolean
        };
        String name;
        Kind type;
        GLuint location;
        
        Uniform(Kind kind, GLuint location);
        
        void setValue(Matrix4x4f value);
        void setValue(Vector4f value);
        void setValue(Vector3f value);
        void setValue(int value);
        void setValue(bool value);
    };
    
private:
    GLuint shaderProgramID;
    GLuint vertexShaderID;
    GLuint fragmentShaderID;

    GLuint stride;

    class VertexInput {
    public:
        GLuint location;
        GLuint size;
        GLenum type;
    };
    std::vector<VertexInput> inputVars;

    std::vector<Uniform*> uniformVars;

public:
    Shader(const String& shaderFolder);
    ~Shader();
    
    void addVec2VertexInput(const String& name);
    void addVec3VertexInput(const String& name);
    void addVec4VertexInput(const String& name);
    Uniform* getMat4Uniform(const String& name);
    Uniform* getVec4fUniform(const String& name);
    Uniform* getVec3fUniform(const String& name);
    Uniform* getIntUniform(const String& name);
    Uniform* getBoolUniform(const String& name);
    void use() const;
    void unbindVertexInputs();
};

#endif // SHADER_H_INCLUDED
