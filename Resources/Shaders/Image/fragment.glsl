#version 330 core

in vec2 fsUV;

uniform sampler2D tex0;

out vec4 outColor;

void main() {
    float depthValue = texture(tex0, fsUV).r;
    outColor = vec4(vec3(depthValue), 1.0);
}
