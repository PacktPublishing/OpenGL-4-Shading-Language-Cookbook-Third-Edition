#ifndef SCENEPARTICLESFEEDBACK_H
#define SCENEPARTICLESFEEDBACK_H

#include "scene.h"
#include "glslprogram.h"
#include "cookbookogl.h"
#include "random.h"
#include "grid.h"

#include <glm/glm.hpp>

class SceneParticlesFeedback : public Scene
{
private:
    GLSLProgram prog, flatProg;

    Random rand;

    // Position and direction of particle emitter
    glm::vec3 emitterPos, emitterDir;

    // Particle buffers
    GLuint posBuf[2], velBuf[2], age[2];
    // Particle VAOs
    GLuint particleArray[2];
    // Transform feedbacks
    GLuint feedback[2];

    GLuint drawBuf;
    Grid grid;

    int nParticles;
    float particleLifetime;
    float angle;
    float time, deltaT;

    void setMatrices(GLSLProgram & shader);
    void compileAndLinkShader();
    void initBuffers();

public:
    SceneParticlesFeedback();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEPARTICLESFEEDBACK_H
