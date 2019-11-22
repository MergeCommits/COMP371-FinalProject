#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

#include "Vector.h"
#include "Rectangle.h"

class Line2f {
public:
    Vector2f pointA; Vector2f pointB;

    Line2f();
    Line2f(const Vector2f& a, const Vector2f& b);
    Line2f(float ax, float ay, float bx, float by);

    Rectanglef boundingBox() const;
    bool intersects(const Line2f& other, Vector2f& point) const;
    Vector2f closestPoint(const Vector2f& point) const;
};

#endif // LINE_H_INCLUDED