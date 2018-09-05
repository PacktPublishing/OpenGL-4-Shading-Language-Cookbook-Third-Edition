#version 400

const float PI = 3.14159265359;

uniform int Pass;

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

layout (location = 3) in vec3 ParticlePosition;
layout (location = 4) in vec3 ParticleVelocity;
layout (location = 5) in float ParticleAge;
layout (location = 6) in vec2 ParticleRotation;

// To transform feedback
/*layout( xfb_buffer = 0, xfb_offset=0 )*/ out vec3 Position;
/*layout( xfb_buffer = 1, xfb_offset=0 )*/ out vec3 Velocity;
/*layout( xfb_buffer = 2, xfb_offset=0 )*/ out float Age;
/*layout( xfb_buffer = 3, xfb_offset=0 )*/ out vec2 Rotation;

// To fragment shader
out vec3 fPosition;
out vec3 fNormal;

uniform float Time;               // Simulation time
uniform float DeltaT;             // Elapsed time between frames
uniform vec3 Accel;               // Particle acceleration (gravity)
uniform float ParticleLifetime;   // Particle lifespan
uniform vec3 Emitter = vec3(0);   // World position of the emitter.
uniform mat3 EmitterBasis;        // Rotation that rotates y axis to the direction of emitter
uniform float ParticleSize;       // Size of particle

// Transforms
uniform mat4 MV;    // View * Model
uniform mat4 Proj;  // Projection matrix

uniform sampler1D RandomTex;

vec3 randomInitialVelocity() {
    float theta = mix(0.0, PI / 6.0, texelFetch(RandomTex, 4 * gl_VertexID, 0).r );
    float phi = mix(0.0, 2.0 * PI, texelFetch(RandomTex, 4 * gl_VertexID + 1, 0).r);
    float velocity = mix(1.25, 1.5, texelFetch(RandomTex, 4 * gl_VertexID + 2, 0).r );
    vec3 v = vec3(sin(theta) * cos(phi), cos(theta), sin(theta) * sin(phi));
    return normalize(EmitterBasis * v) * velocity;
}

float randomInitialRotationalVelocity() {
    return mix(-15.0, 15.0, texelFetch(RandomTex, 4 * gl_VertexID + 3, 0).r );
}

void update() {
    if( ParticleAge < 0 || ParticleAge > ParticleLifetime ) {
        // The particle is past it's lifetime, recycle.
        Position = Emitter;
        Velocity = randomInitialVelocity();
        Rotation = vec2( 0.0, randomInitialRotationalVelocity() );
        if( ParticleAge < 0 ) Age = ParticleAge + DeltaT;
        else Age = (ParticleAge - ParticleLifetime) + DeltaT;
    } else {
        // The particle is alive, update.
        Position = ParticlePosition + ParticleVelocity * DeltaT;
        Velocity = ParticleVelocity + Accel * DeltaT;
        Rotation.x = mod( ParticleRotation.x + ParticleRotation.y * DeltaT, 2.0 * PI );
        Rotation.y = ParticleRotation.y;
        Age = ParticleAge + DeltaT;
    }
}

void render() {
    float cs = cos(ParticleRotation.x);
    float sn = sin(ParticleRotation.x);
    mat4 rotationAndTranslation = mat4(
        1, 0, 0, 0,
        0, cs, sn, 0,
        0, -sn, cs, 0,
        ParticlePosition.x, ParticlePosition.y, ParticlePosition.z, 1
    );
    mat4 m = MV * rotationAndTranslation;
    fPosition = (m * vec4(VertexPosition, 1)).xyz;
    fNormal = (m * vec4(VertexNormal, 0)).xyz;

    // Draw at the current position
    gl_Position = Proj * vec4(fPosition, 1.0);
}

void main() {
    if( Pass == 1 )
        update();
    else
        render();
}
