#ifndef SCENEPARTICLES_H
#define SCENEPARTICLES_H

#include "scene.h"
#include "grid.h"
#include "glslprogram.h"
#include "cookbookogl.h"
#include "random.h"

#include <glm/glm.hpp>
#include <random>

class SceneParticles : public Scene
{
private:
    Random rand;

    GLSLProgram prog, flatProg;
    GLuint initVel, startTime, particles, nParticles;

    Grid grid;

    // Position and direction of particle emitter
    glm::vec3 emitterPos, emitterDir;

    float angle, time, particleLifetime;

    void setMatrices(GLSLProgram &);
    void compileAndLinkShader();
    void initBuffers();
    float randFloat();

public:
    SceneParticles();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEPARTICLES_H
