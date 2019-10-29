#version 330 core

in vec3 position;
in vec3 normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
    // The OpenGL shader compiler optimizes out unused input variables
    // so we have to use them here even though they ultimately do nothing.
    if (normal.x < 0.0) {
        gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0f);
    } else {
        gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0f);
    }
}
