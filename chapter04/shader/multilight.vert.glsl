#version 410

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 Color;

uniform struct LightInfo {
  vec4 Position;  // Light position in eye coords.
  vec3 La;        // Ambient light intesity
  vec3 L;         // Diffuse and specular light intensity
} lights[5];

uniform struct MaterialInfo {
  vec3 Ka;            // Ambient reflectivity
  vec3 Kd;            // Diffuse reflectivity
  vec3 Ks;            // Specular reflectivity
  float Shininess;    // Specular shininess factor
} Material;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

vec3 phongModel( int light, vec3 position, vec3 n ) {  
  vec3 ambient = lights[light].La * Material.Ka;
  vec3 s = normalize( lights[light].Position.xyz - position );
  float sDotN = max( dot(s,n), 0.0 );
  vec3 diffuse = Material.Kd * sDotN;
  vec3 spec = vec3(0.0);
  if( sDotN > 0.0 ) {
    vec3 v = normalize(-position.xyz);
    vec3 r = reflect( -s, n );
    spec = Material.Ks *
            pow( max( dot(r,v), 0.0 ), Material.Shininess );
  }

  return ambient + lights[light].L * (diffuse + spec);
}

void main()
{
    vec3 camNorm = normalize( NormalMatrix * VertexNormal);
    vec3 camPosition = (ModelViewMatrix * vec4(VertexPosition,1.0)).xyz;

    // Evaluate the lighting equation, for each light
    Color = vec3(0.0);
    for( int i = 0; i < 5; i++ )
        Color += phongModel( i, camPosition, camNorm );

    gl_Position = MVP * vec4(VertexPosition,1.0);
}
