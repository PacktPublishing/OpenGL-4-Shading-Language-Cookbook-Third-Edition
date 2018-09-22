#pragma once

#include "scene.h"
#include "glslprogram.h"
#include "teapot.h"
#include "skybox.h"

#include <glm/glm.hpp>

class SceneRust : public Scene
{
private:
	GLSLProgram prog, skyProg;

    Teapot teapot;
    SkyBox sky;

    float angle, tPrev, rotSpeed;

    void setMatrices(GLSLProgram &);
    void compileAndLinkShader();

public:
    SceneRust();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};
