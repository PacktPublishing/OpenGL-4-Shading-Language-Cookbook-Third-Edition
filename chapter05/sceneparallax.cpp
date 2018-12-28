
#include "sceneparallax.h"
#include "texture.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

SceneParallax::SceneParallax() : plane(8, 8, 1, 1) , tPrev(0) {}

void SceneParallax::initScene()
{
	compileAndLinkShader();
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	model = glm::mat4(1.0f);
	angle = glm::radians(90.0f);

	// Load textures
	normalMap = Texture::loadTexture("../media/texture/mybrick/mybrick-normal.png");
	heightMap = Texture::loadTexture("../media/texture/mybrick/mybrick-height.png");
	colorMap = Texture::loadTexture("../media/texture/mybrick/mybrick-color.png");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, heightMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	view = glm::lookAt(glm::vec3(-1, 0, 8), glm::vec3(-1,0,0), glm::vec3(0, 1, 0));

	prog.use();
	prog.setUniform("Light.L", glm::vec3(0.7f));
	prog.setUniform("Light.La", glm::vec3(0.01f));
	prog.setUniform("Material.Ks", glm::vec3(0.7f));
	prog.setUniform("Material.Shininess", 40.0f);
}

void SceneParallax::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	prog.setUniform("Light.Position", view * glm::vec4(2.0f, 2.0f, 1.0f, 1.0f));

	model = glm::rotate(glm::mat4(1.0f), glm::radians(65.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
	
	setMatrices(prog);
	plane.render();
}

void SceneParallax::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	projection = glm::perspective(glm::radians(35.0f), (float)w / h, 0.3f, 100.0f);
}

void SceneParallax::update(float t)
{
	float deltaT = t - tPrev;
	if (tPrev == 0.0f) deltaT = 0.0f;
	tPrev = t;
	angle = glm::mod(angle + deltaT * (1.5f), glm::two_pi<float>());
	if (animating()) {}
}


void SceneParallax::setMatrices(GLSLProgram &p)
{
	glm::mat4 mv = view * model;
	p.setUniform("ModelViewMatrix", mv);
	p.setUniform("NormalMatrix", glm::mat3(mv));
	p.setUniform("MVP", projection * mv);
}

void SceneParallax::compileAndLinkShader()
{
	try {
		//prog.compileShader("shader/parallax.vert.glsl");
		//prog.compileShader("shader/parallax.frag.glsl");
		//prog.link();

		prog.compileShader("shader/steep-parallax.vert.glsl");
		prog.compileShader("shader/steep-parallax.frag.glsl");
		prog.link();
	}
	catch (GLSLProgramException & e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

