#include <stdlib.h>
#include <math.h>

#include "Line.h"
#include "MathUtil.h"

Line2f::Line2f() {
    pointA = Vector2f::zero; pointB = Vector2f::zero;
}

Line2f::Line2f(const Vector2f& a, const Vector2f& b) {
    pointA = a; pointB = b;
}

Line2f::Line2f(float ax, float ay, float bx, float by) {
    pointA = Vector2f(ax, ay); pointB = Vector2f(bx, by);
}

Rectanglef Line2f::boundingBox() const {
    return Rectanglef(pointA,pointB);
}

bool Line2f::intersects(const Line2f& other, Vector2f& point) const {
    Vector2f p1a = pointA;
    Vector2f p1b = pointB;
    
    Vector2f p2a = other.pointA;
    Vector2f p2b = other.pointB;

    float denominator = ((p1a.x - p1b.x) * (p2a.y - p2b.y)) - ((p1a.y - p1b.y) * (p2a.x - p2b.x));
    // Are they lines parallel?
    if (fabs(denominator) < MathUtil::MARGIN_ERROR) { return false; }

    point.x = (((p1a.x * p1b.y) - (p1a.y * p1b.x)) * (p2a.x - p2b.x)) - ((p1a.x - p1b.x) * ((p2a.x * p2b.y) - (p2a.y * p2b.x)));
    point.y = (((p1a.x * p1b.y) - (p1a.y * p1b.x)) * (p2a.y - p2b.y)) - ((p1a.y - p1b.y) * ((p2a.x * p2b.y) - (p2a.y * p2b.x)));
    point = point.multiply(1.f / denominator);

    return boundingBox().isPointInside(point) && other.boundingBox().isPointInside(point);
}

Vector2f Line2f::closestPoint(const Vector2f& point) const {
    Vector2f aToP = point.subtract(pointA);
    Vector2f aToB = pointB.subtract(pointA);

    float t = aToP.dotProduct(aToB) / aToB.lengthSquared();

    if (t < 0) { return pointA; }
    if (t > 1) { return pointB; }
    
    return pointA.add(aToB.multiply(t));
}
