#version 430

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec3 HdrColor;

uniform int Pass;  // Pass number

uniform struct LightInfo {
  vec4 Position;  // Light position in eye coords.
  vec3 L; // D,S intensity
  vec3 La; // Ambient
} Lights[3];

uniform struct MaterialInfo {
  vec3 Ka;
  vec3 Kd;            // Diffuse reflectivity
  vec3 Ks;
  float Shininess;
} Material;

uniform float AveLum;

layout(binding=0) uniform sampler2D HdrTex;

// XYZ/RGB conversion matrices from:
// http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html

uniform mat3 rgb2xyz = mat3( 
  0.4124564, 0.2126729, 0.0193339,
  0.3575761, 0.7151522, 0.1191920,
  0.1804375, 0.0721750, 0.9503041 );

uniform mat3 xyz2rgb = mat3(
  3.2404542, -0.9692660, 0.0556434,
  -1.5371385, 1.8760108, -0.2040259,
  -0.4985314, 0.0415560, 1.0572252 );

uniform float Exposure = 0.35;
uniform float White = 0.928;
uniform bool DoToneMap = true;

vec3 blinnPhong( vec3 position, vec3 n, int idx ) {
  vec3 ambient = Lights[idx].La * Material.Ka;
  vec3 s = normalize( Lights[idx].Position.xyz - position );
  float sDotN = max( dot(s,n), 0.0 );
  vec3 diffuse = Material.Kd * sDotN;
  vec3 spec = vec3(0.0);
  if( sDotN > 0.0 ) {
    vec3 v = normalize(-position.xyz);
    vec3 h = normalize( v + s );
    spec = Material.Ks *
            pow( max( dot(h,n), 0.0 ), Material.Shininess );
  }
  return ambient + Lights[idx].L * (diffuse + spec);
}

void pass1() {
  vec3 n = normalize(Normal);
  // Compute shading and store result in high-res framebuffer
  HdrColor = vec3(0.0);
  for( int i = 0; i < 3; i++)
    HdrColor += blinnPhong(Position, n, i); 
}

// This pass computes the sum of the luminance of all pixels
void pass2() {
    // Retrieve high-res color from texture
    vec4 color = texture( HdrTex, TexCoord );
    
    // Convert to XYZ
    vec3 xyzCol = rgb2xyz * vec3(color);

    // Convert to xyY
    float xyzSum = xyzCol.x + xyzCol.y + xyzCol.z;
    vec3 xyYCol = vec3( xyzCol.x / xyzSum, xyzCol.y / xyzSum, xyzCol.y);

    // Apply the tone mapping operation to the luminance (xyYCol.z or xyzCol.y)
    float L = (Exposure * xyYCol.z) / AveLum;
    L = (L * ( 1 + L / (White * White) )) / ( 1 + L );

    // Using the new luminance, convert back to XYZ
    xyzCol.x = (L * xyYCol.x) / (xyYCol.y);
    xyzCol.y = L;
    xyzCol.z = (L * (1 - xyYCol.x - xyYCol.y))/xyYCol.y;

    // Convert back to RGB and send to output buffer
    if( DoToneMap )
      FragColor = vec4( xyz2rgb * xyzCol, 1.0);
    else
      FragColor = color;
}

void main() {
  if( Pass == 1 ) pass1();
  else if( Pass == 2) pass2();
}
