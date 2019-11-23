#include "Wheel.h"
#include "Mesh.h"
#include "../Utils/OBJ_Loader.h"
#include "../Math/MathUtil.h"

Wheel::Wheel(Shader* shd) {
    GLuint err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Uncaught exception - Wheel(Shader* shd).");
    }
    
    mesh = new Mesh(shd);
    setShader(shd);
    
    // Initialize Loader
    objl::Loader loader;
    bool loaded = loader.LoadFile("Textures/wheel.obj");

    if (loaded) {
        for (int i = 0; i < (int)loader.LoadedMeshes.size(); i++) {
            objl::Mesh currMesh = loader.LoadedMeshes[i];

            // Convert the array of indices from unsigned to signed integers.
            std::vector<int> prims(currMesh.Indices.begin(), currMesh.Indices.end());

            // Convert the array of vertices to our Vector type.
            std::vector<float> verts;
            for (int i = 0; i < (int)currMesh.Vertices.size(); i++) {
                verts.push_back(currMesh.Vertices[i].Position.X);
                verts.push_back(currMesh.Vertices[i].Position.Y + 40.7f);
                verts.push_back(currMesh.Vertices[i].Position.Z);
                verts.push_back(currMesh.Vertices[i].Normal.X);
                verts.push_back(currMesh.Vertices[i].Normal.Y);
                verts.push_back(currMesh.Vertices[i].Normal.Z);
                verts.push_back((std::rand() % 10) / 10.f);
                verts.push_back((std::rand() % 10) / 10.f);
//                verts.push_back(currMesh.Vertices[i].TextureCoordinate.X);
//                verts.push_back(currMesh.Vertices[i].TextureCoordinate.Y);
            }

            mesh->setGeometry(verts, prims);
        }
    } else {
        std::cerr << "Something went wrong.\n";
    }
    
    color = Vector4f(1.f, 0.f, 0.f, 1.f);
    scale = Vector3f(0.025f, 0.025f, 0.025f);
    scaleOrigin = Vector3f::one;
    rotation.y = MathUtil::PI / 2.f;
    tireRotation = 0.f;
}

void Wheel::setPosition(float x, float y, float z) {
    position = Vector3f(x, y, z);
}

void Wheel::addPositionXZ(const Vector2f& vect) {
    position.x += vect.x;
    position.z += vect.y;
}

void Wheel::addRotationX(float bruh) {
    rotation.x += bruh;
}

void Wheel::addRotationZ(float bruh) {
    rotation.z += bruh;
}

void Wheel::setTireRotation(float bruh) {
    tireRotation = bruh;
}

void Wheel::setShader(Shader* shd) {
    mesh->setShader(shd);
    worldMatrixUniform = shd->getMat4Uniform("modelMatrix");
    colorUniform = shd->getVec4fUniform("fsColor");
}

void Wheel::update(const Matrix4x4f& originWorldMatrix) {
    Matrix4x4f scaleMat = Matrix4x4f::scale(scale);
    
    // Wheel center is 0,0,0. Push it up to 0,0.5,0 since that's where they should be rotated about.
    Vector3f localOrigin = Vector3f(0.f, 0.5f, 0.f);
    Matrix4x4f rotationMat = Matrix4x4f::rotate(Vector3f(rotation.x, rotation.y, rotation.z), localOrigin);
    // Do tire rotations after. Otherwise the rotations are out of order and the "moving" animation of the wheels is on the wrong axis.
    Matrix4x4f tireRotateMat = Matrix4x4f::rotate(Vector3f(0.f, tireRotation, 0.f), localOrigin);
    rotationMat = rotationMat.product(tireRotateMat);
    
    worldMatrix = scaleMat.product(rotationMat.product(Matrix4x4f::translate(position).product(originWorldMatrix)));
}

void Wheel::render() {
    worldMatrixUniform->setValue(worldMatrix);
    colorUniform->setValue(color);
    glCullFace(GL_FRONT);
    mesh->render();
    glCullFace(GL_BACK);
}
