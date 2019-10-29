#version 330 core

in vec2 position;
in vec2 uv;

out vec2 fsUV;

void main() {
    gl_Position = vec4(position.xy, 0.5f, 1.0f);
    fsUV = uv;
}
