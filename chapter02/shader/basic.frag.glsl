#version 410

layout(location=0) in vec3 vColor;
layout (location=0) out vec4 FragColor;

void main() {
    FragColor = vec4(vColor, 1.0);
}
