#pragma once

#include "scene.h"
#include "glslprogram.h"
#include "objmesh.h"
#include "skybox.h"

#include <glm/glm.hpp>

class SceneDiffIbl : public Scene
{
private:
	GLSLProgram prog, sbProg;
	GLuint diffCube, colorTex, cube;

	std::unique_ptr<ObjMesh> spot;
	SkyBox skyBox;

	float camAngle, tPrev, rotSpeed;
	glm::vec3 camPos;

	void setMatrices(GLSLProgram &);
	void compileAndLinkShader();

public:
	SceneDiffIbl();

	void initScene();
	void update(float t);
	void render();
	void resize(int, int);
};
