#version 410

const float PI = 3.141592653589793;

layout(location=0) in vec3 Color;
layout (location=0) out vec4 FragColor;

void main() {
    float fac = 1.0 + 0.5 * cos( gl_FragCoord.x * 100 / PI );
    FragColor = vec4(fac * Color, 1.0);
}
