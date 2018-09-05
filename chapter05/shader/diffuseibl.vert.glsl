#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

out vec3 Position; // world coords
out vec3 Normal;  // In world coords.
out vec2 TexCoord;

uniform mat4 ModelViewMatrix;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

void main() {
    TexCoord = VertexTexCoord;
    Position = (ModelMatrix * vec4(VertexPosition,1)).xyz;
    Normal = normalize( ModelMatrix * vec4(VertexNormal,0) ).xyz;
    gl_Position = MVP * vec4(VertexPosition,1.0);
}
