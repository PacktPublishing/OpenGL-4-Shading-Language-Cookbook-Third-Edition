#version 410

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexColor;

layout (location=0) out vec3 vColor;

void main()
{
    vColor = VertexColor;

    gl_Position = vec4(VertexPosition,1.0);
}
