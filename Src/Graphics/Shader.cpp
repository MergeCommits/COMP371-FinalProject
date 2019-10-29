#include <fstream>
#include <iostream>

#include "Shader.h"

Shader::Shader(const String& shaderFolder) {
    shaderProgramID = glCreateProgram();
    stride = 0;

    // Vertex.
    String vertexSource;
    std::ifstream vertexSourceFile; vertexSourceFile.open((shaderFolder + "vertex.glsl").cstr());
    if (!vertexSourceFile.good()) {
        throw std::runtime_error("Failed to load vertex shader source.");
    }

    char* buf = new char[512];
    while (!vertexSourceFile.eof()) {
        vertexSourceFile.read(buf, 511);
        buf[vertexSourceFile.gcount()] = '\0';
        vertexSource = String(vertexSource, buf);
    }
    delete[] buf;

    vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    const char* cstr = vertexSource.cstr();
    glShaderSource(vertexShaderID, 1, &cstr, nullptr);
    glCompileShader(vertexShaderID);
    glAttachShader(shaderProgramID, vertexShaderID);

    int success;
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        throw std::runtime_error("Failed to compile vertex shader.");
    }

    // Fragment.
    String fragmentSource;
    std::ifstream fragmentSourceFile; fragmentSourceFile.open((shaderFolder + "fragment.glsl").cstr());
    if (!fragmentSourceFile.good()) {
        throw std::runtime_error("Failed to load fragment shader source.");
    }

    buf = new char[512];
    while (!fragmentSourceFile.eof()) {
        fragmentSourceFile.read(buf, 511);
        buf[fragmentSourceFile.gcount()] = '\0';
        fragmentSource = String(fragmentSource, buf);
    }
    delete[] buf;

    fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    cstr = fragmentSource.cstr();
    glShaderSource(fragmentShaderID, 1, &cstr, nullptr);
    glCompileShader(fragmentShaderID);
    glAttachShader(shaderProgramID, fragmentShaderID);

    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        throw std::runtime_error("Failed to compile fragment shader.");
    }

    glLinkProgram(shaderProgramID);
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        throw std::runtime_error("Failed to link shader.");

    }
}

Shader::~Shader() {
    glDeleteProgram(shaderProgramID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    for (int i = 0; i < (int)uniformVars.size(); i++) {
        delete uniformVars[i];
    }
}

void Shader::addVec2VertexInput(const String& name) {
    VertexInput vi;
    vi.location = glGetAttribLocation(shaderProgramID, name.cstr());
    vi.type = GL_FLOAT;
    vi.size = 2;

    inputVars.push_back(vi);
    stride += sizeof(GLfloat) * vi.size;
    
    GLuint err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Failed to create vec2 vertex input.");
    }
}

void Shader::addVec3VertexInput(const String& name) {
    VertexInput vi;
    vi.location = glGetAttribLocation(shaderProgramID, name.cstr());
    vi.type = GL_FLOAT;
    vi.size = 3;

    inputVars.push_back(vi);
    stride += sizeof(GLfloat) * vi.size;
    
    GLuint err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Failed to create vec3 vertex input.");
    }
}

void Shader::addVec4VertexInput(const String& name) {
    VertexInput vi;
    vi.location = glGetAttribLocation(shaderProgramID, name.cstr());
    vi.type = GL_FLOAT;
    vi.size = 4;

    inputVars.push_back(vi);
    stride += sizeof(GLfloat) * vi.size;
    
    GLuint err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Failed to create vec4 vertex input.");
    }
}

Shader::Uniform* Shader::getMat4Uniform(const String& name) {
    for (int i = 0; i < (int)uniformVars.size(); i++) {
        if (uniformVars[i]->name.equals(name)) {
            if (uniformVars[i]->type != Uniform::Kind::Matrix) {
                throw std::runtime_error("Attempted to assign mat4 value to non-mat4 type!");
            }
            return uniformVars[i];
        }
    }

    // This isn't here, make it.
    Uniform* uf = new Uniform(Uniform::Kind::Matrix, glGetUniformLocation(shaderProgramID, name.cstr()));
    uf->name = name;
    uniformVars.push_back(uf);
    return uf;
}

Shader::Uniform* Shader::getVec4fUniform(const String& name) {
    for (int i = 0; i < (int)uniformVars.size(); i++) {
        if (uniformVars[i]->name.equals(name)) {
            if (uniformVars[i]->type != Uniform::Kind::Vector4f) {
                throw std::runtime_error("Attempted to assign vec4 value to non-vec4 type!");
            }
            return uniformVars[i];
        }
    }

    // This isn't here, make it.
    Uniform* uf = new Uniform(Uniform::Kind::Vector4f, glGetUniformLocation(shaderProgramID, name.cstr()));
    uf->name = name;
    uniformVars.push_back(uf);
    return uf;
}

Shader::Uniform* Shader::getVec3fUniform(const String& name) {
    for (int i = 0; i < (int)uniformVars.size(); i++) {
        if (uniformVars[i]->name.equals(name)) {
            if (uniformVars[i]->type != Uniform::Kind::Vector3f) {
                throw std::runtime_error("Attempted to assign vec3 value to non-vec3 type!");
            }
            return uniformVars[i];
        }
    }

    // This isn't here, make it.
    Uniform* uf = new Uniform(Uniform::Kind::Vector3f, glGetUniformLocation(shaderProgramID, name.cstr()));
    uf->name = name;
    uniformVars.push_back(uf);
    return uf;
}

Shader::Uniform* Shader::getIntUniform(const String& name) {
    for (int i = 0; i < (int)uniformVars.size(); i++) {
        if (uniformVars[i]->name.equals(name)) {
            if (uniformVars[i]->type != Uniform::Kind::Integer) {
                throw std::runtime_error("Attempted to assign int value to non-int type!");
            }
            return uniformVars[i];
        }
    }

    // This isn't here, make it.
    Uniform* uf = new Uniform(Uniform::Kind::Integer, glGetUniformLocation(shaderProgramID, name.cstr()));
    uf->name = name;
    
    uniformVars.push_back(uf);
    return uf;
}

Shader::Uniform* Shader::getBoolUniform(const String& name) {
    for (int i = 0; i < (int)uniformVars.size(); i++) {
        if (uniformVars[i]->name.equals(name)) {
            if (uniformVars[i]->type != Uniform::Kind::Boolean) {
                throw std::runtime_error("Attempted to assign bool value to non-bool type!");
            }
            return uniformVars[i];
        }
    }

    // This isn't here, make it.
    Uniform* uf = new Uniform(Uniform::Kind::Boolean, glGetUniformLocation(shaderProgramID, name.cstr()));
    uf->name = name;
    
    uniformVars.push_back(uf);
    return uf;
}

void Shader::use() const {
    GLuint err = GL_NO_ERROR;
    glUseProgram(shaderProgramID);

    // Set up vertex attributes.
    uint8_t* ptr = nullptr;
    for (int i = 0; i < (int)inputVars.size(); i++) {
        glEnableVertexAttribArray(inputVars[i].location);
        err = glGetError();
        if (err != GL_NO_ERROR) {
            throw std::runtime_error("Failed to assign vertex attribute!");
        }
        glVertexAttribPointer(inputVars[i].location, inputVars[i].size, inputVars[i].type, GL_FALSE, stride, (void*)ptr);
        if (inputVars[i].type == GL_FLOAT) {
            ptr += sizeof(GLfloat) * inputVars[i].size;
        } else {
            throw std::runtime_error("This ain't implemented!");
        }

        err = glGetError();
        if (err != GL_NO_ERROR) {
            throw std::runtime_error("Failed to assign vertex attribute!");
        }
    }

    // Set uniforms.
    for (int i = 0; i < (int)uniformVars.size(); i++) {
        Uniform* uf = uniformVars[i];
        switch (uf->type) {
            case Uniform::Kind::Matrix: {
                glUniformMatrix4fv(uf->location, 1, GL_FALSE, (const float*)uf->value.matrixVal.elements);
            } break;
            case Uniform::Kind::Vector4f: {
                glUniform4f(uf->location, uf->value.vec4Val.x, uf->value.vec4Val.y, uf->value.vec4Val.z, uf->value.vec4Val.w);
            } break;
            case Uniform::Kind::Vector3f: {
                glUniform3f(uf->location, uf->value.vec3Val.x, uf->value.vec3Val.y, uf->value.vec3Val.z);
            } break;
            case Uniform::Kind::Integer: {
                glUniform1i(uf->location, uf->value.intVal);
            } break;
            case Uniform::Kind::Boolean: {
                glUniform1i(uf->location, (int)uf->value.boolVal);
            } break;
        }

        err = glGetError();
        if (err != GL_NO_ERROR) {
            throw std::runtime_error("Failed to assign shader uniform!");
        }
    }
}

void Shader::unbindVertexInputs() {
    for (int i = 0; i < (int)inputVars.size(); i++) {
        glDisableVertexAttribArray(inputVars[i].location);
    }
}

Shader::Uniform::Values::Values() { }

Shader::Uniform::Uniform(Kind kind, GLuint location) {
    type = kind;
    this->location = location;
}

void Shader::Uniform::setValue(Matrix4x4f value) {
    if (type != Kind::Matrix) {
        throw std::runtime_error("Attempted to assign mat4 value to non-mat4 type!");
    }
    this->value.matrixVal = value;
}

void Shader::Uniform::setValue(Vector4f value) {
    if (type != Kind::Vector4f) {
        throw std::runtime_error("Attempted to assign vec4 value to non-vec4 type!");
    }
    this->value.vec4Val = value;
}

void Shader::Uniform::setValue(Vector3f value) {
    if (type != Kind::Vector3f) {
        throw std::runtime_error("Attempted to assign vec3 value to non-vec3 type!");
    }
    this->value.vec3Val = value;
}

void Shader::Uniform::setValue(int value) {
    if (type != Kind::Integer) {
        throw std::runtime_error("Attempted to assign int value to non-int type!");
    }
    this->value.intVal = value;
}

void Shader::Uniform::setValue(bool value) {
    if (type != Kind::Boolean) {
        throw std::runtime_error("Attempted to assign bool value to non-bool type!");
    }
    this->value.boolVal = value;
}
