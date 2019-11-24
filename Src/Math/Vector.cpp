#include <math.h>

#include "Vector.h"

const Vector2f Vector2f::zero = Vector2f(0.f,0.f);
const Vector2f Vector2f::one = Vector2f(1.f,1.f);

Vector2f::Vector2f() {
    x=0.f; y=0.f;
}

Vector2f::Vector2f(float s) {
    x = s; y = s;
}

Vector2f::Vector2f(float ix,float iy) {
    x = ix; y = iy;
}

float Vector2f::lengthSquared() const {
    return x*x+y*y;
}

float Vector2f::length() const {
    return (float)sqrt(x*x+y*y);
}

float Vector2f::distanceSquared(const Vector2f& b) const {
    return subtract(b).lengthSquared();
}

float Vector2f::distance(const Vector2f& b) const {
    return subtract(b).length();
}

bool Vector2f::equals(const Vector2f& b,float epsilon) const {
    return distanceSquared(b)<=(epsilon*epsilon);
}

Vector2f Vector2f::add(const Vector2f& b) const {
    return Vector2f(x+b.x,y+b.y);
}

Vector2f Vector2f::subtract(const Vector2f& b) const {
    return Vector2f(x-b.x,y-b.y);
}

Vector2f Vector2f::multiply(float s) const {
    return Vector2f(x*s,y*s);
}

Vector2f Vector2f::normalize() const {
    return multiply(1.f/length());
}

Vector2f Vector2f::negate() const {
    return Vector2f(-x, -y);
}

Vector2f Vector2f::reflect(const Vector2f& n) const {
    Vector2f reflectedVector = normalize();
    reflectedVector = n.multiply(2.f*reflectedVector.dotProduct(n)).subtract(reflectedVector);
    return reflectedVector.normalize();
}

float Vector2f::dotProduct(const Vector2f& b) const {
    return x*b.x+y*b.y;
}

float Vector2f::crossProduct(const Vector2f& b) const {
    return x*b.y-y*b.x;
}

const Vector3f Vector3f::zero = Vector3f(0.f,0.f,0.f);
const Vector3f Vector3f::one = Vector3f(1.f,1.f,1.f);

Vector3f::Vector3f() {
    x=0.f; y=0.f; z=0.f;
}

Vector3f::Vector3f(float s) {
    x = s; y = s; z = s;
}

Vector3f::Vector3f(float ix,float iy,float iz) {
    x=ix; y=iy; z=iz;
}

float Vector3f::lengthSquared() const {
    return x*x+y*y+z*z;
}

float Vector3f::length() const {
    return (float)sqrt(x*x+y*y+z*z);
}

float Vector3f::distanceSquared(const Vector3f& b) const {
    return subtract(b).lengthSquared();
}

float Vector3f::distance(const Vector3f& b) const {
    return subtract(b).length();
}

Vector3f Vector3f::add(const Vector3f& b) const {
    return Vector3f(x+b.x,y+b.y,z+b.z);
}

Vector3f Vector3f::subtract(const Vector3f& b) const {
    return Vector3f(x-b.x,y-b.y,z-b.z);
}

Vector3f Vector3f::multiply(float s) const {
    return Vector3f(x*s,y*s,z*s);
}

Vector3f Vector3f::normalize() const {
    if (length() == 0) { return Vector3f::zero; }
    return multiply(1.f/length());
}

Vector3f Vector3f::invert() const {
    return Vector3f(-x, -y, -z);
}

Vector3f Vector3f::reflect(const Vector3f& n) const {
    Vector3f reflectedVector = normalize();
    reflectedVector = n.multiply(2.f*reflectedVector.dotProduct(n)).subtract(reflectedVector);
    return reflectedVector.normalize();
}

float Vector3f::dotProduct(const Vector3f& b) const {
    return x*b.x+y*b.y+z*b.z;
}

Vector3f Vector3f::crossProduct(const Vector3f& b) const {
    return Vector3f(y*b.z-z*b.y,z*b.x-x*b.z,x*b.y-y*b.x);
}

Vector4f::Vector4f() {
    x = 0.f; y = 0.f; z = 0.f; w = 0.f;
}

Vector4f::Vector4f(float s) {
    x = s; y = s; z = s; w = s;
}

Vector4f::Vector4f(float ix,float iy,float iz,float iw) {
    x = ix; y = iy; z = iz; w = iw;
}

const Vector4f Vector4f::zero = Vector4f(0.f,0.f,0.f,0.f);
const Vector4f Vector4f::one = Vector4f(1.f,1.f,1.f,1.f);
