#include <cmath>
#include <stdexcept>

#include "Camera.h"
#include "../Math/MathUtil.h"

Camera::Camera(int w, int h, float fov, float nearZ, float farZ, bool orthographic) {
    position = Vector3f(0.f, 0.f, 0.f);
    lookAt = Vector3f(0.f, 0.f, 1.f);
    upDir = Vector3f(0.f, 1.f, 0.f);

    viewMatrix = Matrix4x4f::constructViewMat(position, lookAt, upDir);

    xAngle = 0.f;
    yAngle = 0.f;
    yAngleLimit = MathUtil::PI / 2.f;
    tilt = 0.f;

    this->nearPlaneZ = nearZ;
    this->farPlaneZ = farZ;
    this->fov = fov;
    this->width = w;
    this->height = h;
    this->orthographicProj = orthographic;
    this->thirdPerson = true;
    this->thirdPersonRadius = 5.f;

    rotation = Matrix4x4f::identity;

    needsViewUpdate = true;
    needsProjUpdate = true;
}

Camera::Camera(int w, int h) : Camera(w, h, MathUtil::degToRad(70.f)) { }

void Camera::addShader(Shader* shd) {
    shaders.push_back(shd);

    needsViewUpdate = true;
    needsProjUpdate = true;
}

Matrix4x4f Camera::getViewMatrix() const {
    return viewMatrix;
}

Matrix4x4f Camera::getProjectionMatrix() const {
    return projectionMatrix;
}

void Camera::update() {
    GLuint err = GL_NO_ERROR;
    err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Uncaught exception - Camera::update().");
    }
    if (needsViewUpdate) {
        if (!thirdPerson) {
            rotation = Matrix4x4f::rotate(Vector3f(-yAngle, xAngle, tilt));
            viewMatrix = Matrix4x4f::constructViewMat(position, rotation.transform(lookAt), rotation.transform(upDir));
        } else {
            float cosXAngle = std::cos(xAngle);
            float cosYAngle = std::cos(yAngle);
            float sinXAngle = std::sin(xAngle);
            float sinYAngle = std::sin(yAngle);
            
            Vector3f thirdPersonPosition = position.subtract(Vector3f(thirdPersonRadius * cosYAngle * cosXAngle,
                                                                      thirdPersonRadius * sinYAngle,
                                                                      -thirdPersonRadius * cosYAngle * sinXAngle));
            
            viewMatrix = Matrix4x4f::constructViewMat(thirdPersonPosition, position.subtract(thirdPersonPosition).normalize(), upDir);
        }

        // Update shaders.
        for (int i = 0; i < (int)shaders.size(); i++) {
            shaders[i]->getMat4Uniform("viewMatrix")->setValue(viewMatrix);
        }

        needsViewUpdate = false;
    }

    if (needsProjUpdate) {
        if (!orthographicProj) {
            projectionMatrix = Matrix4x4f::constructPerspectiveMat(fov, getAspectRatio(), nearPlaneZ, farPlaneZ);
        } else {
            projectionMatrix = Matrix4x4f::constructOrthographicMat((float)width, (float)height, nearPlaneZ, farPlaneZ);
        }
        
        for (int i = 0; i < (int)shaders.size(); i++) {
            shaders[i]->getMat4Uniform("projectionMatrix")->setValue(projectionMatrix);
        }

        needsProjUpdate = false;
    }
}

Vector3f Camera::getPosition() const {
    return position;
}

void Camera::setPosition(const Vector3f& pos) {
    needsViewUpdate = true;
    position = pos;
}

void Camera::setTilt(float rad) {
    needsViewUpdate = !MathUtil::eqFloats(rad, tilt);
    tilt = rad;
}

void Camera::addAngle(float x, float y) {
    if (MathUtil::eqFloats(x, 0.f) && MathUtil::eqFloats(y, 0.f)) {
        return;
    }

    xAngle += x;
    yAngle -= y;

    if (yAngle < -yAngleLimit) {
        yAngle = -yAngleLimit;
    } else if (yAngle > yAngleLimit) {
        yAngle = yAngleLimit;
    }

    float PI_MUL_2 = MathUtil::PI * 2.f;

    if (xAngle > PI_MUL_2) {
        xAngle -= PI_MUL_2;
    } else if (xAngle < -PI_MUL_2) {
        xAngle += PI_MUL_2;
    }

    needsViewUpdate = true;
}

void Camera::resetAngle() {
    xAngle = 0.f;
    yAngle = 0.f;
    
    needsViewUpdate = true;
}

void Camera::setThirdPersonPerspective(bool bruh) {
    needsViewUpdate = bruh != thirdPerson;
    thirdPerson = bruh;
}

void Camera::addFov(float deg) {
    fov += MathUtil::degToRad(deg);
    fov = MathUtil::clampFloat(fov, 0.1f, 2.9f);
    
    needsProjUpdate = true;
}

void Camera::setXYClippings(int w, int h) {
    if (width == w && height == h) {
        return;
    }
    
    this->width = w;
    this->height = h;
    
    needsProjUpdate = true;
}

void Camera::setZClippings(float nearZ, float farZ) {
    if (MathUtil::eqFloats(nearPlaneZ, nearZ) && MathUtil::eqFloats(farPlaneZ, farZ)) {
        return;
    }
    
    nearPlaneZ = nearZ;
    farPlaneZ = farZ;
    
    needsProjUpdate = true;
}

float Camera::getAspectRatio() const {
    return (float)width / height;
}

void Camera::setOrthographicProj(bool bruh) {
    needsProjUpdate = bruh != orthographicProj;
    orthographicProj = bruh;
}
