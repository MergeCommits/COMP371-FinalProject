#ifndef RECTCOLLIDER_H_INCLUDED
#define RECTCOLLIDER_H_INCLUDED

#include "../Math/Vector.h"
#include "../Math/Matrix.h"

class RectCollider {
private:
    Vector2f topLeft;
    Vector2f topRight;
    Vector2f bottomLeft;
    Vector2f bottomRight;
    
    Matrix4x4f worldMat;
    
public:
    enum class CollisionDir {
        None = -1,
        Top = 0x1,
        Right = 0x2,
        Bottom = 0x4,
        Left = 0x8
    };
    
    bool collides(const Matrix4x4f& worldMatrix, float& intersectionDeepness) const;
};

#endif // RECTCOLLIDER_H_INCLUDED
