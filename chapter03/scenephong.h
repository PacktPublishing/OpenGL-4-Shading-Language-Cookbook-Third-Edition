#pragma once

#include "scene.h"
#include "glslprogram.h"
#include "torus.h"
#include "cookbookogl.h"

class ScenePhong : public Scene
{
private:
    GLSLProgram prog;

    Torus torus;
    float angle;

    void setMatrices();
    void compileAndLinkShader();

public:
    ScenePhong();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};
