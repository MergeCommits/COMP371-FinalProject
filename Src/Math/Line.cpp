/**
Modified from https://github.com/juanjp600/pge.

Copyright (c) 2019 Juan Pablo Arce

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
**/

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

static bool all4BooleansEqual(bool arr[4]) {
    bool firstValue = arr[0];
    for (int i = 1; i < 4; i++) {
        if (arr[i] != firstValue) {
            return false;
        }
    }
    return true;
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
		bool allXDiffs[4] = {
			other.pointA.x - pointA.x < 0,
			other.pointA.x - pointB.x < 0,
			other.pointB.x - pointA.x < 0,
			other.pointB.x - pointB.x < 0
		};
		if (!all4BooleansEqual(allXDiffs)) {
			return true;
		}

        bool allYDiffs[4] = {
            other.pointA.y - pointA.y < 0,
            other.pointA.y - pointB.y < 0,
            other.pointB.y - pointA.y < 0,
            other.pointB.y - pointB.y < 0
        };
        return !all4BooleansEqual(allYDiffs);
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
