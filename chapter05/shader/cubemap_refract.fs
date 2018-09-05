#version 430

in vec3 ReflectDir;
in vec3 RefractDir;

layout(binding=0) uniform samplerCube CubeMapTex;

struct MaterialInfo {
    float Eta;              // Index of refraction
    float ReflectionFactor; // Percentage of reflected light
};
uniform MaterialInfo Material;

layout( location = 0 ) out vec4 FragColor;

void main() {
    // Access the cube map texture
    vec3 reflectColor = texture(CubeMapTex, ReflectDir).rgb;
    vec3 refractColor = texture(CubeMapTex, RefractDir).rgb;

    vec3 color = mix(refractColor, reflectColor, Material.ReflectionFactor);
    // Gamma
    color = pow(color, vec3(1.0/2.2));
    
    FragColor = vec4(color , 1);
}
