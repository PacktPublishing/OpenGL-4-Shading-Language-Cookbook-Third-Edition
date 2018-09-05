#include "scenediffibl.h"
#include "texture.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "stb/stb_image.h"

SceneDiffIbl::SceneDiffIbl() : tPrev(0.0f), camAngle(glm::half_pi<float>()), rotSpeed(0.5f), camPos(0, 4, 7)
{
}

void SceneDiffIbl::initScene()
{
	compileAndLinkShader();

	spot = ObjMesh::load("../media/spot/spot_triangulated.obj");

	glEnable(GL_DEPTH_TEST);
	
	model = glm::mat4(1.0f);

	//stbi_set_flip_vertically_on_load(true);
	diffCube = Texture::loadHdrCubeMap("../media/texture/cube/grace-diffuse/grace-diffuse");
	//stbi_set_flip_vertically_on_load(false);
	cube = Texture::loadHdrCubeMap("../media/texture/cube/grace/grace");
	colorTex = Texture::loadTexture("../media/spot/spot_texture.png");

}

void SceneDiffIbl::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	model = glm::mat4(1.0f);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cube);
	sbProg.use();
	setMatrices(sbProg);
	skyBox.render();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, diffCube);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, colorTex);
	prog.use();
	prog.setUniform("CamPos", camPos);
	prog.setUniform("Material.Color", glm::vec3(0.4f, 0.4f, 0.4f));
	model = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0));
	setMatrices(prog);
	spot->render();
}

void SceneDiffIbl::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	projection = glm::perspective(glm::radians(50.0f), (float)w / h, 0.3f, 100.0f);
}

void SceneDiffIbl::update(float t)
{
	float deltaT = t - tPrev;
	if (tPrev == 0.0f) deltaT = 0.0f;
	tPrev = t;
	if (animating()) {
		camAngle = glm::mod(camAngle + deltaT * rotSpeed, glm::two_pi<float>());
		camPos.x = glm::cos(camAngle) * 4.0f;
		camPos.y = 0.0f;
		camPos.z = glm::sin(camAngle) * 4.0f;
	}
	view = glm::lookAt(
		camPos,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
}


void SceneDiffIbl::setMatrices(GLSLProgram &p)
{
	glm::mat4 mv = view * model;
	p.setUniform("ModelMatrix", model);
	p.setUniform("ModelViewMatrix", mv);
	p.setUniform("NormalMatrix", glm::mat3(mv));
	p.setUniform("MVP", projection * mv);
}

void SceneDiffIbl::compileAndLinkShader()
{
	try {
		prog.compileShader("shader/diffuseibl.vert.glsl");
		prog.compileShader("shader/diffuseibl.frag.glsl");
		prog.link();

		sbProg.compileShader("shader/skybox.vert.glsl");
		sbProg.compileShader("shader/skybox.frag.glsl");
		sbProg.link();
	}
	catch (GLSLProgramException & e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

