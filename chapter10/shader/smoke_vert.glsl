#version 410

const float PI = 3.14159265359;

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexVelocity;
layout (location = 2) in float VertexAge;

uniform int Pass;

// Out to transform feedback buffers (first pass)
out vec3 Position;
out vec3 Velocity;
out float Age;

// Out to fragment shader (Second pass)
out float Transp;
out vec2 TexCoord;

uniform float Time;                      // Simulation time
uniform float DeltaT;                    // Elapsed time between frames
uniform vec3 Accel;                      // Particle acceleration
uniform float ParticleLifetime;          // Particle lifespan
uniform float MinParticleSize = 0.1;     // Minimum size
uniform float MaxParticleSize = 2.5;     // Maximum size
uniform mat3 EmitterBasis;
uniform vec3 Emitter;

// Transforms
uniform mat4 MV;
uniform mat4 Proj;

uniform sampler1D RandomTex;

// Offsets to the position in camera coordinates for each vertex of the particle's quad
const vec3 offsets[] = vec3[](vec3(-0.5,-0.5,0), vec3(0.5,-0.5,0), vec3(0.5,0.5,0),
                              vec3(-0.5,-0.5,0), vec3(0.5,0.5,0), vec3(-0.5,0.5,0) );
// Texture coordinates for each vertex of the particle's quad
const vec2 texCoords[] = vec2[](vec2(0,0), vec2(1,0), vec2(1,1), vec2(0,0), vec2(1,1), vec2(0,1));

vec3 randomInitialVelocity() {
    float theta = mix(0.0, PI / 1.5, texelFetch(RandomTex, 3 * gl_VertexID, 0).r );
    float phi = mix(0.0, 2.0 * PI, texelFetch(RandomTex, 3 * gl_VertexID + 1, 0).r);
    float velocity = mix(0.1, 0.2, texelFetch(RandomTex, 3 * gl_VertexID + 2, 0).r );
    vec3 v = vec3(sin(theta) * cos(phi), cos(theta), sin(theta) * sin(phi));
    return normalize(EmitterBasis * v) * velocity;
}

void update() {
    if( VertexAge < 0 || VertexAge > ParticleLifetime ) {
        // The particle is past it's lifetime, recycle.
        Position = Emitter;
        Velocity = randomInitialVelocity();
        if( VertexAge < 0 ) Age = VertexAge + DeltaT;
        else Age = (VertexAge - ParticleLifetime) + DeltaT;
    } else {
        // The particle is alive, update.
        Position = VertexPosition + VertexVelocity * DeltaT;
        Velocity = VertexVelocity + Accel * DeltaT;
        Age = VertexAge + DeltaT;
    }
}

void render() {
    Transp = 0.0;
    vec3 posCam = vec3(0.0);
    if( VertexAge >= 0.0 ) {
        float agePct = VertexAge / ParticleLifetime;
        Transp = clamp(1.0 - agePct, 0, 1);
        posCam =
            (MV * vec4(VertexPosition,1)).xyz +
            offsets[gl_VertexID] *
            mix(MinParticleSize, MaxParticleSize, agePct);
    }
    TexCoord = texCoords[gl_VertexID];

    gl_Position = Proj * vec4(posCam,1);
}

void main() {
    if( Pass == 1 )
        update();
    else
        render();
}
