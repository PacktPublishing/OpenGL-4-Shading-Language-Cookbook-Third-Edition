#ifndef SCENEFIRE_H
#define SCENEFIRE_H

#include "scene.h"
#include "glslprogram.h"
#include "cookbookogl.h"
#include "grid.h"

#include <glm/glm.hpp>

class SceneFire : public Scene
{
private:
    GLSLProgram prog, flatProg;

    GLuint posBuf[2], velBuf[2], age[2];
    GLuint particleArray[2];
    GLuint feedback[2];
    GLuint drawBuf;

    Grid grid;

    int nParticles;

    float angle;
    float time, deltaT;
    float particleSize, particleLifetime;

    void setMatrices(GLSLProgram &);
    void compileAndLinkShader();
    void initBuffers();

public:
    SceneFire();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif
