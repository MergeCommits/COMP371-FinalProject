#include <FreeImageIO.h>
#include <stdexcept>

#include "Texture.h"
#include "Shader.h"

Texture::Texture(const String& path) {
    filePath = path;
    
    // Load image.
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filePath.cstr(), 0);
    FIBITMAP* image = FreeImage_Load(format, filePath.cstr());
    FIBITMAP* image32bits = FreeImage_ConvertTo32Bits(image);
    
    // Get an available texture index from OpenGL.
    textureID = 0;
    glGenTextures(1, &textureID);
    if (textureID == 0) {
        std::runtime_error(String("Failed to generate texture: ", path).cstr());
    }
    
    // Set OpenGL filtering properties (bi-linear interpolation).
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
    
    // Retrieve width and height.
    int width = FreeImage_GetWidth(image32bits);
    int height = FreeImage_GetHeight(image32bits);
    
    // Upload the texture to the GPU memory.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height,
                 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image32bits));
    
    // Free images.
    FreeImage_Unload(image);
    FreeImage_Unload(image32bits);
}

Texture::~Texture() {
    glDeleteTextures(1, &textureID);
}

void Texture::activate(int index) const {
    GLenum glTextureLayers[] = {
        GL_TEXTURE0,
        GL_TEXTURE1,
        GL_TEXTURE2,
        GL_TEXTURE3,
        GL_TEXTURE4,
        GL_TEXTURE5,
        GL_TEXTURE6,
        GL_TEXTURE7
    };

    glActiveTexture(glTextureLayers[index]);
    glBindTexture(GL_TEXTURE_2D, textureID);
}
