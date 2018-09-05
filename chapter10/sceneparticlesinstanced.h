#ifndef SCENEPARTICLESINSTANCED_H
#define SCENEPARTICLESINSTANCED_H

#include "scene.h"
#include "glslprogram.h"
#include "cookbookogl.h"
#include "torus.h"
#include "grid.h"

#include <glm/glm.hpp>

class SceneParticlesInstanced : public Scene
{
private:
    GLSLProgram prog;

    // Particle buffers
    GLuint posBuf[2], velBuf[2], age[2], rotation[2];
    // Particle VAOs
    GLuint particleArray[2];
    // Transform feedbacks
    GLuint feedback[2];

    Torus torus;
    Grid grid;

    int nParticles;
    float particleLifetime;
    float angle;
    float time, deltaT;
    int drawBuf;
    glm::vec3 emitterPos, emitterDir;

    void setMatrices(GLSLProgram &);
    void compileAndLinkShader();
    void initBuffers();

public:
    SceneParticlesInstanced();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEPARTICLESINSTANCED_H
