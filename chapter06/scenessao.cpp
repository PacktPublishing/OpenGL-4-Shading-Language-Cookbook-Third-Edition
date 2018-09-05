#include "scenessao.h"
#include "random.h"
#include "texture.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

SceneSsao::SceneSsao() : deferredFBO(0), quad(0), tPrev(0), rotSpeed(1.0f), plane(10, 10, 1, 1, 10, 7) {}

void SceneSsao::initScene()
{
	compileAndLinkShader();

	glEnable(GL_DEPTH_TEST);

	//bunny = ObjMesh::load("../media/bunny/bunny.obj");
	bunny = ObjMesh::load("../media/dragon.obj");

	float c = 1.5f;

	angle = glm::pi<float>() / 2.0f;

	sceneProj = glm::perspective(glm::radians(50.0f), (float)width / height, 0.3f, 100.0f);

	// Array for quad
	GLfloat verts[] = {
		-1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f
	};
	GLfloat tc[] = {
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
	};

	// Set up the buffers
	unsigned int handle[2];
	glGenBuffers(2, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), verts, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), tc, GL_STATIC_DRAW);

	// Set up the vertex array object
	glGenVertexArrays(1, &quad);
	glBindVertexArray(quad);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);  // Texture coordinates

	glBindVertexArray(0);

	woodTex = Texture::loadTexture("../media/texture/hardwood2_diffuse.jpg");
	brickTex = Texture::loadTexture("../media/texture/brick1.jpg");

	prog.setUniform("Light.L", glm::vec3(0.3f));
	prog.setUniform("Light.La", glm::vec3(0.5f));

	// Create and assign the random sample kernel
	buildKernel();

	// Create the texture of random rotation directions
	GLuint rotTex = buildRandRotationTex();
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, rotTex);

	// Setup the G-Buffers
	setupFBO();

#ifdef __APPLE__
	prog.setUniform("PositionTex", 0);
	prog.setUniform("NormalTex", 1);
	prog.setUniform("ColorTex", 2);
#endif
}

GLuint SceneSsao::buildRandRotationTex() {
	int size = 4;
	std::vector<GLfloat> randDirections(3 * size * size);
	for (int i = 0; i < size * size; i++) {
		glm::vec3 v = rand.uniformCircle();
		randDirections[i * 3 + 0] = v.x;
		randDirections[i * 3 + 1] = v.y;
		randDirections[i * 3 + 2] = v.z;
	}

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB16F, size, size);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size, size, GL_RGB, GL_FLOAT, randDirections.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	return tex;
}

void SceneSsao::createGBufTex(GLenum texUnit, GLenum format, GLuint &texid) {
	glActiveTexture(texUnit);
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
#ifdef __APPLE__
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
#else
	glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
#endif
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
}

void SceneSsao::buildKernel() {
	int kernSize = 64;
	std::vector<float> kern(3 * kernSize);
	for (int i = 0; i < kernSize; i++) {
		glm::vec3 randDir = rand.uniformHemisphere();
		float scale = ((float)(i * i)) / (kernSize * kernSize);
		randDir *= glm::mix(0.1f, 1.0f, scale);

		kern[i * 3 + 0] = randDir.x;
		kern[i * 3 + 1] = randDir.y;
		kern[i * 3 + 2] = randDir.z;
		//std::cout << randDir.x << ", " << randDir.y << ", " << randDir.z << std::endl;
	}

	GLuint progHandle = prog.getHandle();
	GLint loc = glGetUniformLocation(progHandle, "SampleKernel");
	glUniform3fv(loc, kernSize, kern.data());
}

void SceneSsao::setupFBO()
{
	GLuint depthBuf, posTex, normTex, colorTex;

	// Create the textures for position, normal and color
	createGBufTex(GL_TEXTURE0, GL_RGB32F, posTex);  // Position
	createGBufTex(GL_TEXTURE1, GL_RGB32F, normTex); // Normal
	createGBufTex(GL_TEXTURE2, GL_RGB8, colorTex);  // Color

	createGBufTex(GL_TEXTURE3, GL_R16F, aoTex[0]);     // AO pair
	createGBufTex(GL_TEXTURE3, GL_R16F, aoTex[1]);
	    
	// Create and bind the FBO
	glGenFramebuffers(1, &deferredFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, deferredFBO);

	// The depth buffer
	glGenRenderbuffers(1, &depthBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	// Attach the textures to the framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, posTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, colorTex, 0);

	GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2, GL_NONE };
	glDrawBuffers(5, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Create and bind the FBO
	glGenFramebuffers(1, &ssaoFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

	// Attach the texture to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, aoTex[0], 0);

	GLenum drawBuffers2[] = { GL_NONE, GL_NONE, GL_NONE, GL_NONE, GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(5, drawBuffers2);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneSsao::update(float t)
{
	float deltaT = t - tPrev;
	if (tPrev == 0.0f) deltaT = 0.0f;
	tPrev = t;

	angle += rotSpeed * deltaT;
	if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
}

void SceneSsao::render()
{
	pass1();  // Render to G-Buffers
	pass2();  // SSAO
	pass3();  // Blur
	pass4();  // Lighting
}

void SceneSsao::pass1()
{
	prog.setUniform("Pass", 1);

	glBindFramebuffer(GL_FRAMEBUFFER, deferredFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	view = glm::lookAt(
		glm::vec3(2.1f, 1.5f, 2.1f), 
		glm::vec3(0.0f, 1.0f, 0.0f), 
		glm::vec3(0.0f, 1.0f, 0.0f));
	projection = sceneProj;

	drawScene();
}

void SceneSsao::pass2()
{
	prog.setUniform("Pass", 2);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

	// Write to aoTex[0]
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, aoTex[0], 0);

	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	prog.setUniform("ProjectionMatrix", sceneProj);

	drawScreenFillingQuad();
}

// Blur pass
void SceneSsao::pass3()
{
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	prog.setUniform("Pass", 3);

	// Read from aoTex[0], write to aoTex[1]
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, aoTex[0]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, aoTex[1], 0);

	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	drawScreenFillingQuad();
}

// Final color pass
void SceneSsao::pass4()
{
	prog.setUniform("Pass", 4);

	// Read from aoTex[1] (blurred)
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, aoTex[1]);

	// Revert to default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	drawScreenFillingQuad();
}

void SceneSsao::drawScreenFillingQuad() {
	view = glm::mat4(1.0);
	model = glm::mat4(1.0);
	projection = glm::mat4(1.0);
	setMatrices();

	// Render the quad
	glBindVertexArray(quad);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void SceneSsao::drawScene() {

	prog.setUniform("Light.Position", view * glm::vec4(3.0f, 3.0f, 1.5f, 1.0f));

	// Walls
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, woodTex);
	prog.setUniform("Material.UseTex", 1);
	model = glm::mat4(1.0f);
	setMatrices();
	plane.render();

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, brickTex);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -2));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
	setMatrices();
	plane.render();

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-2, 0, 0));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
	setMatrices();
	plane.render();

	prog.setUniform("Material.UseTex", 0);
	prog.setUniform("Material.Kd", glm::vec3(0.9f, 0.5f, 0.2f));
	model = glm::rotate(glm::mat4(1.0f), glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f));
	model = glm::translate(model, glm::vec3(0, 0.282958, 0));

	setMatrices();
	bunny->render();
}

void SceneSsao::setMatrices()
{
	glm::mat4 mv = view * model;
	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("NormalMatrix", glm::mat3(mv));
	prog.setUniform("MVP", projection * mv);
}

void SceneSsao::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
}

void SceneSsao::compileAndLinkShader()
{
	try {
#ifdef __APPLE__
		prog.compileShader("shader/deferred_41.vs");
		prog.compileShader("shader/deferred_41.fs");
#else
		prog.compileShader("shader/ssao.vert.glsl");
		prog.compileShader("shader/ssao.frag.glsl");
#endif
		prog.link();
		prog.use();
	}
	catch (GLSLProgramException &e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}
