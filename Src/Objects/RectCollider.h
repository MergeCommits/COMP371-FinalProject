#ifndef RECTCOLLIDER_H_INCLUDED
#define RECTCOLLIDER_H_INCLUDED

#include "../Math/Vector.h"
#include "../Math/Matrix.h"
#include "../Math/Line.h"

class RectCollider {
private:
    Vector2f topLeft;
    Vector2f topRight;
    Vector2f bottomLeft;
    Vector2f bottomRight;
    
    Line2f top;
    Line2f right;
    Line2f bottom;
    Line2f left;
    
    Matrix4x4f worldMat;
    
    static Vector2f transformXZCoordinates(const Vector2f& xzCoordinates, const Matrix4x4f& worldMatrix);
    static bool lineSegmentIntersectsCollider(const Line2f& line, const RectCollider& other, float& intersectionDeepness);
    
public:
    enum class CollisionDir {
        None = -1,
        Top = 0x1,
        Right = 0x2,
        Bottom = 0x4,
        Left = 0x8
    };
    
    RectCollider()=default;
    RectCollider(const Vector2f& tl, const Vector2f& tr, const Vector2f& bl, const Vector2f& br);
    
    bool collides(const RectCollider& other, CollisionDir& collisionSide) const;
    
    void update(const Matrix4x4f& worldMatrix);
};

#endif // RECTCOLLIDER_H_INCLUDED
