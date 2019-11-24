#include "RectCollider.h"
#include "../Primitives/Axis.h"
#include "../Graphics/Mesh.h"

static void pushNormalsToVector(std::vector<float>& vect) {
    vect.push_back(0.f);
    vect.push_back(1.f);
    vect.push_back(0.f);
}

RectCollider::RectCollider(const Vector2f& tl, const Vector2f& tr, const Vector2f& bl, const Vector2f& br, Shader* shd) {
    topLeft = tl;
    topRight = tr;
    bottomLeft = bl;
    bottomRight = br;
    
    GLuint err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Uncaught exception - RectCollider(Shader* shd).");
    }
    
    mesh = new Mesh(shd);
    const float LINE_HEIGHT = 4.f;
    std::vector<float> verts;
    
    verts.push_back(topLeft.x);
    verts.push_back(LINE_HEIGHT);
    verts.push_back(topLeft.y);
    pushNormalsToVector(verts);
    
    verts.push_back(topRight.x);
    verts.push_back(LINE_HEIGHT);
    verts.push_back(topRight.y);
    pushNormalsToVector(verts);
    
    verts.push_back(bottomRight.x);
    verts.push_back(LINE_HEIGHT);
    verts.push_back(bottomRight.y);
    pushNormalsToVector(verts);
    
    verts.push_back(bottomLeft.x);
    verts.push_back(LINE_HEIGHT);
    verts.push_back(bottomLeft.y);
    pushNormalsToVector(verts);
    
    std::vector<int> prims = {
        0, 1,
        1, 2,
        2, 3,
        3, 0
    };
    
    mesh->setGeometry(verts, prims, GL_LINES);

    worldMatrixUniform = shd->getMat4Uniform("modelMatrix");
    colorUniform = shd->getVec4fUniform("fsColor");
}

RectCollider::~RectCollider() {
    delete axisTop;
    delete axisLeft;
    delete axisBottom;
    delete axisRight;
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
        if (line.intersects(colLines[i])) {
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

void RectCollider::update(const Matrix4x4f& worldMat) {
    worldMatrix = worldMat;
    Vector2f transformedTL = transformXZCoordinates(topLeft, worldMatrix);
    Vector2f transformedTR = transformXZCoordinates(topRight, worldMatrix);
    Vector2f transformedBL = transformXZCoordinates(bottomLeft, worldMatrix);
    Vector2f transformedBR = transformXZCoordinates(bottomRight, worldMatrix);
    
    top = Line2f(transformedTL, transformedTR);
    right = Line2f(transformedTR, transformedBR);
    bottom = Line2f(transformedBL, transformedBR);
    left = Line2f(transformedBL, transformedTL);
}

void RectCollider::render() const {
    worldMatrixUniform->setValue(worldMatrix);
    colorUniform->setValue(Vector4f(0.f, 0.f, 1.f, 1.f));
    
    mesh->render();
}
