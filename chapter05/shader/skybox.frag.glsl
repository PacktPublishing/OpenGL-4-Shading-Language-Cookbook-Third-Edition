#version 430

layout(binding=1) uniform samplerCube SkyBoxTex;
in vec3 Vec;
layout( location = 0 ) out vec4 FragColor;

void main() {
    vec3 texColor = texture(SkyBoxTex, normalize(Vec)).rgb;
    texColor = pow( texColor, vec3(1.0/2.2));
    FragColor = vec4(texColor,1);
}
