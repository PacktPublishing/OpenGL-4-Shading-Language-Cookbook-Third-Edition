#version 430

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec3 PositionData;
layout (location = 2) out vec3 NormalData;
layout (location = 3) out vec3 ColorData;

uniform int Pass;   // Pass number

uniform struct LightInfo {
  vec4 Position;  // Light position in eye coords.
  vec3 L;   // D,S intensity
  vec3 La;  // Ambient
} Light ;

uniform struct MaterialInfo {
  vec3 Kd;            // Diffuse reflectivity
} Material;

layout(binding=0) uniform sampler2D PositionTex;
layout(binding=1) uniform sampler2D NormalTex;
layout(binding=2) uniform sampler2D ColorTex;

vec3 diffuseModel( vec3 pos, vec3 norm, vec3 diff ) {
    vec3 s = normalize( vec3(Light.Position) - pos);
    float sDotN = max( dot(s,norm), 0.0 );
    return Light.L * diff * sDotN;
}

void pass1() {
    // Store position, normal, and diffuse color in textures
    PositionData = Position;
    NormalData = normalize(Normal);
    ColorData = Material.Kd;
}

void pass2() {
    // Retrieve position and normal information from textures
    vec3 pos = vec3( texture( PositionTex, TexCoord ) );
    vec3 norm = vec3( texture( NormalTex, TexCoord ) );
    vec3 diffColor = vec3( texture(ColorTex, TexCoord) );

    FragColor = vec4( diffuseModel(pos,norm,diffColor), 1.0 );
}

void main() {
    if( Pass == 1) pass1();
    else if(Pass==2) pass2();
}
