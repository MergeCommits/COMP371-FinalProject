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

bool Line2f::intersects(const Line2f& other) const {
    Vector2f r = pointB.subtract(pointA);
    Vector2f s = other.pointB.subtract(other.pointA);

    float uNumerator = other.pointA.subtract(pointA).crossProduct(r);
    float denominator = r.crossProduct(s);

    if (MathUtil::eqFloats(uNumerator, 0.f) && MathUtil::eqFloats(denominator, 0.f)) {
        // They are colinear.

        // Any touch?
        bool contact1 = pointA.equals(other.pointA);
        bool contact2 = pointA.equals(other.pointB);
        bool contact3 = pointB.equals(other.pointA);
        bool contact4 = pointB.equals(other.pointB);
        if (contact1 || contact2 || contact3 || contact4) {
            return true;
        }

        // Do they overlap? Are all the point differences in either direction the same sign?
        bool allXHaveSameSign = (other.pointA.x - pointA.x < 0)
            && (other.pointA.x - pointB.x < 0)
            && (other.pointB.x - pointA.x < 0)
            && (other.pointB.x - pointB.x < 0);
        if (allXHaveSameSign) { return true; }

        bool allYHaveSameSign = (other.pointA.y - pointA.y < 0)
            && (other.pointA.y - pointB.y < 0)
            && (other.pointB.y - pointA.y < 0)
            && (other.pointB.y - pointB.y < 0);
        return allYHaveSameSign;
    }

    if (MathUtil::eqFloats(denominator, 0.f)) {
        // Lines are parallel.
        return false;
    }

    float u = uNumerator / denominator;
    float t = other.pointA.subtract(pointA).crossProduct(s) / denominator;

    return (t >= 0) && (t <= 1) && (u >= 0) && (u <= 1);
}

Vector2f Line2f::closestPoint(const Vector2f& point) const {
    Vector2f aToP = point.subtract(pointA);
    Vector2f aToB = pointB.subtract(pointA);

    float t = aToP.dotProduct(aToB) / aToB.lengthSquared();

    if (t < 0) { return pointA; }
    if (t > 1) { return pointB; }

    return pointA.add(aToB.multiply(t));
}
