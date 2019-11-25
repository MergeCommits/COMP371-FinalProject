#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

#include "Shader.h"

class Mesh;

class Sprite {
private:
    Shader::Uniform* modelMatrixUniform;
    Shader::Uniform* scaleUniform;
    Shader::Uniform* rotationMatrixUniform;
    Shader::Uniform* colorUniform;

    Mesh* mesh;

    Vector3f position;
    Vector2f scale;
    float rotation;
    Matrix4x4f modelMatrix;
    // Stores the roll rotations of the sprite.
    Matrix4x4f rotationMatrix;
    Vector4f color;

public:
    Sprite(Shader* shd);
    ~Sprite();

    void setScale(float scale);
    void setPosition(const Vector3f& pos);
    void addRotation(float rad);
    
    void setOpacity(float value);

    void update();
    void render() const;
};

#endif // SPRITE_H_INCLUDED
