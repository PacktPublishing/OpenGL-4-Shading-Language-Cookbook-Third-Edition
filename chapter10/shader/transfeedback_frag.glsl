#version 410

uniform sampler2D ParticleTex;

in float Transp;
in vec2 TexCoord;

layout ( location = 0 ) out vec4 FragColor;

void main()
{
    FragColor = texture(ParticleTex, TexCoord);
    FragColor.a *= Transp;
}
