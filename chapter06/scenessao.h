#pragma once

#include "scene.h"
#include "glslprogram.h"
#include "plane.h"
#include "objmesh.h"
#include "random.h"

class SceneSsao : public Scene
{
private:
	GLSLProgram prog;

	GLuint aoTex[2], woodTex, brickTex;

	GLuint deferredFBO, ssaoFBO, blurFBO;
	GLuint quad;
	Random rand;

	Plane plane;
	std::unique_ptr<ObjMesh> bunny;

	glm::mat4 sceneProj;

	float angle, tPrev, rotSpeed;

	GLuint buildRandRotationTex();
	void setMatrices();
	void compileAndLinkShader();
	void setupFBO();
	void createGBufTex(GLenum, GLenum, GLuint &);
	void buildKernel();
	void pass1();
	void pass2();

	void pass3();

	void pass4();

	void drawScreenFillingQuad();

	void drawScene();

public:
	SceneSsao();

	void initScene();
	void update(float t);
	void render();
	void resize(int, int);
};