#version 430
const float PI = 3.14159265358979323846;

in vec3 Position;
in vec3 Normal;  // World coords.
in vec2 TexCoord;

uniform vec3 CamPos;

layout(binding=0) uniform samplerCube DiffLightTex;
layout(binding=1) uniform sampler2D ColorTex;

uniform struct MaterialInfo {
    vec3 Color;
} Material;

layout( location = 0 ) out vec4 FragColor;


vec3 schlickFresnel( float dotProd ) {
  vec3 f0 = vec3(0.04);
  return f0 + (1 - f0) * pow(1.0 - dotProd, 5);
}

void main() {
    float gamma = 2.2;
    vec3 n = normalize(Normal);
    vec3 v = normalize( CamPos - Position );

    // Look up incoming light from diffuse cube map
    vec3 light = texture(DiffLightTex, n).rgb;
    vec3 color = texture(ColorTex, TexCoord).rgb;

    color = pow(color, vec3(gamma));

    // Uncomment to add an Fresnel approximation
    //color = light * color * (1.0 - schlickFresnel(dot(n, v)));
    color *= light;

    // Gamma
    color = pow( color, vec3(1.0/gamma));

    FragColor = vec4( color, 1 );
}
