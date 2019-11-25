#ifndef RECTCOLLIDER_H_INCLUDED
#define RECTCOLLIDER_H_INCLUDED

#include "../Math/Vector.h"
#include "../Math/Matrix.h"
#include "../Math/Line.h"
#include "../Graphics/Shader.h"

class Mesh;

class RectCollider {
public:
    enum class CollisionDir {
        None = -1,
        Top = 0,
        Right = 1,
        Bottom = 2,
        Left = 3
    };
    
private:
    Vector2f topLeft;
    Vector2f topRight;
    Vector2f bottomLeft;
    Vector2f bottomRight;
    
    Line2f top;
    Line2f right;
    Line2f bottom;
    Line2f left;
    
    Mesh* mesh;
    Matrix4x4f worldMatrix;
    Shader::Uniform* worldMatrixUniform;
    Shader::Uniform* colorUniform;
    
    static Vector2f transformXZCoordinates(const Vector2f& xzCoordinates, const Matrix4x4f& worldMatrix);
    static bool lineSegmentIntersectsCollider(const Line2f& line, const RectCollider* other, CollisionDir& collisionSide);
    
public:
    RectCollider()=default;
    RectCollider(const Vector2f& tl, const Vector2f& tr, const Vector2f& bl, const Vector2f& br, Shader* shd);
    ~RectCollider();
    
    bool collides(const RectCollider* other, CollisionDir& collisionSide) const;
    
    void update(const Matrix4x4f& worldMatrix);
    void render() const;
};

const RectCollider::CollisionDir operator&(const RectCollider::CollisionDir& a, const RectCollider::CollisionDir& b);
const RectCollider::CollisionDir operator|(const RectCollider::CollisionDir& a, const RectCollider::CollisionDir& b);

#endif // RECTCOLLIDER_H_INCLUDED
