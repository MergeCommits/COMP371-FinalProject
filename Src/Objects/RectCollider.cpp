#include "RectCollider.h"

RectCollider::RectCollider(const Vector2f& tl, const Vector2f& tr, const Vector2f& bl, const Vector2f& br) {
    topLeft = tl;
    topRight = tr;
    bottomLeft = bl;
    bottomRight = br;
}

Vector2f RectCollider::transformXZCoordinates(const Vector2f& xzCoordinates, const Matrix4x4f& worldMatrix) {
    Vector3f xyzSpace = Vector3f(xzCoordinates.x, 0.f, xzCoordinates.y);
    Vector3f transform = worldMatrix.transform(xyzSpace);
    
    return Vector2f(transform.x, transform.z);
}

bool RectCollider::lineSegmentIntersectsCollider(const Line2f& line, const RectCollider& other) {
    Line2f colLines[4];
    colLines[0] = other.top;
    colLines[1] = other.right;
    colLines[2] = other.bottom;
    colLines[3] = other.left;
    
    for (int i = 0; i < 4; i++) {
        Vector2f intersectPoint;
        if (line.intersects(colLines[i], intersectPoint)) {
            return true;
        }
    }
    
    return false;
}

bool RectCollider::collides(const RectCollider& other, CollisionDir& collisionSide) const {
    collisionSide = CollisionDir::None;
    
    if (lineSegmentIntersectsCollider(top, other)) {
        collisionSide = CollisionDir::Top;
        return true;
    }
    
    if (lineSegmentIntersectsCollider(right, other)) {
        collisionSide = CollisionDir::Right;
        return true;
    }
    
    if (lineSegmentIntersectsCollider(bottom, other)) {
        collisionSide = CollisionDir::Bottom;
        return true;
    }
    
    if (lineSegmentIntersectsCollider(left, other)) {
        collisionSide = CollisionDir::Left;
        return true;
    }
    
    return false;
}

void RectCollider::update(const Matrix4x4f& worldMatrix) {
    Vector2f transformedTL = transformXZCoordinates(topLeft, worldMatrix);
    Vector2f transformedTR = transformXZCoordinates(topRight, worldMatrix);
    Vector2f transformedBL = transformXZCoordinates(bottomLeft, worldMatrix);
    Vector2f transformedBR = transformXZCoordinates(bottomRight, worldMatrix);
    
    top = Line2f(transformedTL, transformedTR);
    right = Line2f(transformedTR, transformedBR);
    bottom = Line2f(transformedBL, transformedBR);
    left = Line2f(transformedBL, transformedTL);
}
