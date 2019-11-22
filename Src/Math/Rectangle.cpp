#include "Rectangle.h"

Rectanglef::Rectanglef() {
    tl = Vector2f::zero;
    br = Vector2f::zero;
}

Rectanglef::Rectanglef(const Vector2f& itl, const Vector2f& ibr) {
    tl = itl;
    br = ibr;
    
    repair();
}

Rectanglef::Rectanglef(float il, float it, float ir, float ib) {
    tl = Vector2f(il, it);
    br = Vector2f(ir, ib);
    
    repair();
}

void Rectanglef::repair() {
    if (tl.x > br.x) {
        float tlx = tl.x; float brx = br.x;
        br.x = tlx; tl.x = brx;
    }
    if (tl.y > br.y) {
        float tly = tl.y; float bry = br.y;
        br.y = tly; tl.y = bry;
    }
}

float Rectanglef::width() const {
    return br.x - tl.x;
}

float Rectanglef::height() const {
    return br.y - tl.y;
}

float Rectanglef::area() const {
    return width() * height();
}

Vector2f Rectanglef::center() const {
    return tl.add(br).multiply(0.5f);
}

Vector2f Rectanglef::topLeftCorner() const {
    return tl;
}

Vector2f Rectanglef::bottomRightCorner() const {
    return br;
}

Vector2f Rectanglef::topRightCorner() const {
    return Vector2f(br.x, tl.y);
}

Vector2f Rectanglef::bottomLeftCorner() const {
    return Vector2f(tl.x, br.y);
}

bool Rectanglef::isPointInside(const Vector2f& point) const {
    if (point.x<tl.x) return false;
    if (point.y<tl.y) return false;
    if (point.x>br.x) return false;
    if (point.y>br.y) return false;
    return true;
}

bool Rectanglef::intersects(const Rectanglef& other) const {
    if (other.topLeftCorner().x>bottomRightCorner().x) return false;
    if (other.bottomRightCorner().x<topLeftCorner().x) return false;
    if (other.topLeftCorner().y>bottomRightCorner().y) return false;
    if (other.bottomRightCorner().y<topLeftCorner().y) return false;
    return true;
}
