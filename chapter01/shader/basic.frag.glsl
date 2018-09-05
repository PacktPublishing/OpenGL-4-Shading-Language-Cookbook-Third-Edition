#version 410

layout(location=0) in vec3 fColor;
layout (location=0) out vec4 FragColor;

void main() {
    FragColor = vec4(fColor, 1.0);
}
