#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <GL/glew.h>
#include "../Utils/String.h"

class Shader;

class Texture {
private:
    String filePath;
    
    GLuint textureID;
    
public:
    Texture(const String& path);
    ~Texture();
    
    void activate(int index) const;
};

#endif // TEXTURE_H_INCLUDED
