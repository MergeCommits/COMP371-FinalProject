#ifndef RECTANGLE_H_INCLUDED
#define RECTANGLE_H_INCLUDED

#include "Vector.h"

class Rectanglef {
private:
    Vector2f tl; Vector2f br;
    void repair();

public:
    Rectanglef();
    Rectanglef(const Vector2f& itl, const Vector2f& ibr);
    Rectanglef(float il, float it, float ir, float ib);

    float width() const;
    float height() const;
    float area() const;
    
    Vector2f center() const;
    Vector2f topLeftCorner() const;
    Vector2f bottomRightCorner() const;
    Vector2f topRightCorner() const;
    Vector2f bottomLeftCorner() const;

    bool isPointInside(const Vector2f& p) const;
    bool intersects(const Rectanglef& other) const;
};

#endif // RECTANGLE_H_INCLUDED
