#include "scenepbr.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

ScenePbr::ScenePbr() : plane(20, 20, 1, 1), teapot(5, glm::mat4(1.0f)), tPrev(0.0f), lightPos(5.0f, 5.0f, 5.0f, 1.0f) {
	mesh = ObjMesh::load("../media/spot/spot_triangulated.obj");
}

void ScenePbr::initScene() {
	compileAndLinkShader();

	glEnable(GL_DEPTH_TEST);

	view = glm::lookAt(
		glm::vec3(0.0f, 4.0f, 7.0f), 
		glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	projection = glm::perspective(glm::radians(50.0f),(float)width/height, 0.5f, 100.0f);

	lightAngle = 0.0f;
	lightRotationSpeed = 1.5f;

	prog.setUniform("Light[0].L", glm::vec3(45.0f));
	prog.setUniform("Light[0].Position", view * lightPos);
	prog.setUniform("Light[1].L", glm::vec3(0.3f));
	prog.setUniform("Light[1].Position", glm::vec4(0, 0.15f, -1.0f, 0));
	prog.setUniform("Light[2].L", glm::vec3(45.0f));
	prog.setUniform("Light[2].Position", view * glm::vec4(-7, 3, 7, 1));
}

void ScenePbr::update(float t) {
	float deltaT = t - tPrev;
	if (tPrev == 0.0f) deltaT = 0.0f;
	tPrev = t;
	if (animating()) {
		lightAngle = glm::mod( lightAngle + deltaT * lightRotationSpeed, glm::two_pi<float>() );
		lightPos.x = glm::cos(lightAngle) * 7.0f;
		lightPos.y = 3.0f;
		lightPos.z = glm::sin(lightAngle) * 7.0f;
	}
}

void ScenePbr::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	prog.setUniform("Light[0].Position", view * lightPos);
	drawScene();
}

void ScenePbr::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	projection = glm::perspective(glm::radians(60.0f), (float)w / h, 0.3f, 100.0f);
}

void ScenePbr::setMatrices()
{
	glm::mat4 mv = view * model;
	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("NormalMatrix", glm::mat3(mv));
	prog.setUniform("MVP", projection * mv);
}

void ScenePbr::compileAndLinkShader() {
	try {
		prog.compileShader("shader/pbr.vert.glsl");
		prog.compileShader("shader/pbr.frag.glsl");
		prog.link();
		prog.use();
	}
	catch (GLSLProgramException & e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

void ScenePbr::drawScene() {
	drawFloor();

	// Draw dielectric cows with varying roughness
	int numCows = 9;
	glm::vec3 cowBaseColor(0.1f, 0.33f, 0.17f);
	for (int i = 0; i < numCows; i++) {
		float cowX = i * (10.0f / (numCows - 1)) - 5.0f;
		float rough = (i + 1) * (1.0f / numCows);
		drawSpot(glm::vec3(cowX, 0, 0), rough, 0, cowBaseColor);
	}

	// Draw metal cows
	float metalRough = 0.43f;
	// Gold
	drawSpot(glm::vec3(-3.0f, 0.0f, 3.0f), metalRough, 1, glm::vec3(1, 0.71f, 0.29f));
	// Copper
	drawSpot(glm::vec3(-1.5f, 0.0f, 3.0f), metalRough, 1, glm::vec3(0.95f, 0.64f, 0.54f));
	// Aluminum
	drawSpot(glm::vec3(-0.0f, 0.0f, 3.0f), metalRough, 1, glm::vec3(0.91f, 0.92f, 0.92f));
	// Titanium
	drawSpot(glm::vec3(1.5f, 0.0f, 3.0f), metalRough, 1, glm::vec3(0.542f, 0.497f, 0.449f));
	// Silver
	drawSpot(glm::vec3(3.0f, 0.0f, 3.0f), metalRough, 1, glm::vec3(0.95f, 0.93f, 0.88f));

}

void ScenePbr::drawFloor() {
	model = glm::mat4(1.0f);
	prog.setUniform("Material.Rough", 0.9f);
	prog.setUniform("Material.Metal", 0);
	prog.setUniform("Material.Color", glm::vec3(0.2f));
	model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.0f));

	setMatrices();
	plane.render();
}

void ScenePbr::drawSpot(const glm::vec3 & pos, float rough, int metal, const glm::vec3 & color) {
	model = glm::mat4(1.0f);
	prog.setUniform("Material.Rough", rough);
	prog.setUniform("Material.Metal", metal);
	prog.setUniform("Material.Color", color);
	model = glm::translate(model, pos);
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	setMatrices();
	mesh->render();
}