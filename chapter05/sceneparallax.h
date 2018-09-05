#pragma once

#include "scene.h"
#include "plane.h"
#include "glslprogram.h"

class SceneParallax : public Scene {

private:
	GLSLProgram prog;
	Plane plane;

	float tPrev, angle;
	GLuint normalMap, heightMap, colorMap;

	void setMatrices(GLSLProgram &);
	void compileAndLinkShader();

public:
	SceneParallax();

	void initScene();
	void update(float t);
	void render();
	void resize(int, int);
};