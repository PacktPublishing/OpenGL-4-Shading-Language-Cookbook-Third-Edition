#version 410

in vec3 Position;
in vec3 Normal;

uniform struct SpotLightInfo {
    vec3 Position;   // Position in eye coords
    vec3 L;          // Diffuse/spec intensity
    vec3 La;         // Amb intensity
    vec3 Direction;  // Direction of the spotlight in eye coords.
    float Exponent;  // Angular attenuation exponent
    float Cutoff;    // Cutoff angle (between 0 and pi/2)
} Spot;

uniform struct MaterialInfo {
  vec3 Ka;            // Ambient reflectivity
  vec3 Kd;            // Diffuse reflectivity
  vec3 Ks;            // Specular reflectivity
  float Shininess;    // Specular shininess factor
} Material;

layout( location = 0 ) out vec4 FragColor;

vec3 blinnPhongSpot( vec3 position, vec3 n ) {  
  vec3 ambient = Spot.La * Material.Ka, 
    diffuse = vec3(0), spec = vec3(0);
  vec3 s = normalize( Spot.Position - position );
  float cosAng = dot(-s, normalize(Spot.Direction));
  float angle = acos( cosAng );
  float spotScale = 0.0;
  if(angle >= 0.0 && angle < Spot.Cutoff ) {
    spotScale = pow( cosAng, Spot.Exponent );
    float sDotN = max( dot(s,n), 0.0 );
    diffuse = Material.Kd * sDotN;
    if( sDotN > 0.0 ) {
      vec3 v = normalize(-position.xyz);
      vec3 h = normalize( v + s );
      spec = Material.Ks *
        pow( max( dot(h,n), 0.0 ), Material.Shininess );
    }
  }
  return ambient + spotScale * Spot.L * (diffuse + spec);
}

void main() {
  FragColor = vec4(blinnPhongSpot(Position, normalize(Normal)), 1);
}
