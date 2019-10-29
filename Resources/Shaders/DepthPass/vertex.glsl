#version 330 core

in vec3 position;
in vec3 normal;
in vec2 uv;

uniform mat4 depthViewMatrix;
uniform mat4 depthProjectionMatrix;
uniform mat4 modelMatrix;

void main() {
    // The OpenGL shader compiler optimizes out unused input variables
    // so we have to use them here even though they ultimately do nothing.
    if (normal.x < 0.0 && uv.x < 0.0) {
        gl_Position =  depthProjectionMatrix * depthViewMatrix * modelMatrix * vec4(position, 1.0);
    } else {
        gl_Position =  depthProjectionMatrix * depthViewMatrix * modelMatrix * vec4(position, 1.0);
    }
}
