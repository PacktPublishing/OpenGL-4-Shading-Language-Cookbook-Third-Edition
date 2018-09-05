#version 430

in vec3 ReflectDir;

layout(binding=0) uniform samplerCube CubeMapTex;

uniform float ReflectFactor;
uniform vec4 MaterialColor;

layout( location = 0 ) out vec4 FragColor;

void main() {
    // Access the cube map texture
    vec3 cubeMapColor = texture(CubeMapTex, ReflectDir).rgb;
    // Gamma correct
    cubeMapColor = pow(cubeMapColor, vec3(1.0/2.2));
    FragColor = vec4( mix( MaterialColor.rgb, cubeMapColor, ReflectFactor), 1);
}
