#version 430

layout (location = 0) in vec3 VertexPosition;

out vec3 Vec;
uniform mat4 MVP;

void main() {
    Vec = VertexPosition;
    gl_Position = MVP * vec4(VertexPosition,1.0);
}
