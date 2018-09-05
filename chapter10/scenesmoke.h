#ifndef SCENESMOKE_H
#define SCENESMOKE_H

#include "scene.h"
#include "glslprogram.h"
#include "cookbookogl.h"
#include "grid.h"

#include <glm/glm.hpp>

class SceneSmoke : public Scene
{
private:
    GLSLProgram prog, flatProg;

    // Position and direction of emitter.
    glm::vec3 emitterPos, emitterDir;

    GLuint posBuf[2], velBuf[2], age[2];
    GLuint particleArray[2];
    GLuint feedback[2];
    GLuint drawBuf;

    Grid grid;

    int nParticles;
    float angle;
    float time, deltaT;
    float particleLifetime;

    void setMatrices(GLSLProgram & p);
    void compileAndLinkShader();
    void initBuffers();

public:
    SceneSmoke();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENESMOKE_H
