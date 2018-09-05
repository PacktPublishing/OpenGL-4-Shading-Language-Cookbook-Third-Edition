#version 410

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexColor;

uniform vec3 uColorMask = vec3(0.0);

layout (location=0) out vec3 Color;

out gl_PerVertex
{
  vec4 gl_Position;
  float gl_PointSize;
  float gl_ClipDistance[];
};

void main()
{
    Color = VertexColor * uColorMask;

    gl_Position = vec4(VertexPosition,1.0);
}
