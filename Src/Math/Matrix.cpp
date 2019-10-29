#include <math.h>

#include "Matrix.h"

const Matrix4x4f Matrix4x4f::identity = Matrix4x4f(1,0,0,0,
                                                   0,1,0,0,
                                                   0,0,1,0,
                                                   0,0,0,1);

Matrix4x4f::Matrix4x4f() {
    *this = identity;
}

Matrix4x4f::Matrix4x4f(float aa,float ab,float ac,float ad,
                       float ba,float bb,float bc,float bd,
                       float ca,float cb,float cc,float cd,
                       float da,float db,float dc,float dd) {
    elements[0][0] = aa; elements[0][1] = ab; elements[0][2] = ac; elements[0][3] = ad;
    elements[1][0] = ba; elements[1][1] = bb; elements[1][2] = bc; elements[1][3] = bd;
    elements[2][0] = ca; elements[2][1] = cb; elements[2][2] = cc; elements[2][3] = cd;
    elements[3][0] = da; elements[3][1] = db; elements[3][2] = dc; elements[3][3] = dd;
}

Matrix4x4f Matrix4x4f::transpose() const {
    Matrix4x4f retVal;
    for (int i=0;i<4;i++) {
        for (int j=0;j<4;j++) {
            retVal.elements[i][j] = elements[j][i];
        }
    }
    return retVal;
}

Matrix4x4f Matrix4x4f::product(const Matrix4x4f& other) const {
    Matrix4x4f retVal;
    for (int i=0;i<4;i++) {
        for (int j=0;j<4;j++) {
            retVal.elements[i][j] = 0.f;
            for (int k=0;k<4;k++) {
                retVal.elements[i][j] += elements[i][k]*other.elements[k][j];
            }
        }
    }
    return retVal;
}

Vector4f Matrix4x4f::transform(const Vector4f& other) const {
    Vector4f retVal = Vector4f::zero;
    retVal.x = other.x*elements[0][0]+other.y*elements[0][1]+other.z*elements[0][2]+other.w*elements[0][3];
    retVal.y = other.x*elements[1][0]+other.y*elements[1][1]+other.z*elements[1][2]+other.w*elements[1][3];
    retVal.z = other.x*elements[2][0]+other.y*elements[2][1]+other.z*elements[2][2]+other.w*elements[2][3];
    retVal.w = other.x*elements[3][0]+other.y*elements[3][1]+other.z*elements[3][2]+other.w*elements[3][3];
    return retVal;
}

Vector3f Matrix4x4f::transform(const Vector3f& other) const {
    Vector4f retVal(other.x,other.y,other.z,1.f);
    retVal = transform(retVal);
    return Vector3f(retVal.x,retVal.y,retVal.z);
}

Matrix4x4f Matrix4x4f::translate(const Vector3f& position) {
    return Matrix4x4f(1.f,0.f,0.f,0.f,
                      0.f,1.f,0.f,0.f,
                      0.f,0.f,1.f,0.f,
                      position.x,position.y,position.z,1.f);
}

Matrix4x4f Matrix4x4f::scale(const Vector3f& scale) {
    return Matrix4x4f(scale.x,0.f,0.f,0.f,
                      0.f,scale.y,0.f,0.f,
                      0.f,0.f,scale.z,0.f,
                      0.f,0.f,0.f,1.f);
}

Matrix4x4f Matrix4x4f::scale(const Vector3f& scale, const Vector3f& origin) {
    return Matrix4x4f::translate(Vector3f(-origin.x, -origin.y, -origin.z)).product(Matrix4x4f::scale(scale).product(Matrix4x4f::translate(origin)));
}

Matrix4x4f Matrix4x4f::rotate(const Vector3f& rotation) {
    float sinPitch = sin(rotation.x);
    float sinYaw = sin(rotation.y);
    float sinRoll = sin(rotation.z);
    float cosPitch = cos(rotation.x);
    float cosYaw = cos(rotation.y);
    float cosRoll = cos(rotation.z);
    
    Matrix4x4f pitchMat = Matrix4x4f(1.f,0.f,0.f,0.f,
                                     0.f,cosPitch,-sinPitch,0.f,
                                     0.f,sinPitch,cosPitch,0.f,
                                     0.f,0.f,0.f,1.f);
    
    Matrix4x4f yawMat = Matrix4x4f(cosYaw,0.f,sinYaw,0.f,
                                   0.f,1.f,0.f,0.f,
                                   -sinYaw,0.f,cosYaw,0.f,
                                   0.f,0.f,0.f,1.f);
    
    Matrix4x4f rollMat = Matrix4x4f(cosRoll,-sinRoll,0.f,0.f,
                                    sinRoll,cosRoll,0.f,0.f,
                                    0.f,0.f,1.f,0.f,
                                    0.f,0.f,0.f,1.f);
    
    return yawMat.product(pitchMat.product(rollMat));
}

Matrix4x4f Matrix4x4f::rotate(const Vector3f& rotation, const Vector3f& origin) {
    return Matrix4x4f::translate(Vector3f(-origin.x, -origin.y, -origin.z)).product(Matrix4x4f::rotate(rotation).product(Matrix4x4f::translate(origin)));
}

Matrix4x4f Matrix4x4f::constructWorldMat(const Vector3f& position,const Vector3f& scale,const Vector3f& rotation) {
    Matrix4x4f translationMat = translate(position);
    Matrix4x4f scaleMat = Matrix4x4f::scale(scale);
    Matrix4x4f rotationMat = rotate(rotation);

    return scaleMat.product(rotationMat.product(translationMat));
}

Matrix4x4f Matrix4x4f::constructViewMat(const Vector3f& position,const Vector3f& target,const Vector3f& upVector) {
    Matrix4x4f newMat = identity;

    Vector3f zAxis = target.multiply(-1.f);
    zAxis = zAxis.normalize();

    Vector3f xAxis = upVector.crossProduct(zAxis);
    xAxis = xAxis.normalize();

    Vector3f yAxis = zAxis.crossProduct(xAxis);

    newMat.elements[0][0] = xAxis.x;
    newMat.elements[0][1] = yAxis.x;
    newMat.elements[0][2] = zAxis.x;
    newMat.elements[0][3] = 0.f;

    newMat.elements[1][0] = xAxis.y;
    newMat.elements[1][1] = yAxis.y;
    newMat.elements[1][2] = zAxis.y;
    newMat.elements[1][3] = 0;

    newMat.elements[2][0] = xAxis.z;
    newMat.elements[2][1] = yAxis.z;
    newMat.elements[2][2] = zAxis.z;
    newMat.elements[2][3] = 0;

    newMat.elements[3][0] = -xAxis.dotProduct(position);
    newMat.elements[3][1] = -yAxis.dotProduct(position);
    newMat.elements[3][2] = -zAxis.dotProduct(position);
    newMat.elements[3][3] = 1;

    return newMat;
}

Matrix4x4f Matrix4x4f::constructPerspectiveMat(float horizontalfov, float aspectRatio, float nearZ, float farZ) {
    Matrix4x4f retval = Matrix4x4f::identity;
    float rad = horizontalfov * 0.5f;
    float nad = cos(rad) / sin(rad);

    retval.elements[0][0] = nad * (-1.0f / aspectRatio);
    retval.elements[1][1] = nad;
    retval.elements[2][2] = farZ / (nearZ - farZ);
    retval.elements[2][3] = -1.f;
    retval.elements[3][2] = (nearZ * farZ / (nearZ - farZ));

    return retval;
}

Matrix4x4f Matrix4x4f::constructOrthographicMat(float width, float height, float nearZ, float farZ) {
    Matrix4x4f retval = Matrix4x4f::identity;

    retval.elements[0][0] = 2.f / width;
    retval.elements[1][1] = 2.f / height;
    retval.elements[2][2] = 1.f / (farZ - nearZ);

    return retval;
}
