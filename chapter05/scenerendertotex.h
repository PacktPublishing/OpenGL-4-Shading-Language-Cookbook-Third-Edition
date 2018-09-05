#ifndef SCENERENDERTOTEX_H
#define SCENERENDERTOTEX_H

#include "scene.h"
#include "glslprogram.h"
#include "cube.h"
#include "objmesh.h"

#include <glm/glm.hpp>

class SceneRenderToTex : public Scene
{
private:
    GLSLProgram prog;

    GLuint fboHandle;
    Cube cube;
    std::unique_ptr<ObjMesh> spot;

    float angle;
    float tPrev, rotSpeed;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();
    void renderToTexture();
    void renderScene();

public:
    SceneRenderToTex();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENERENDERTOTEX_H
