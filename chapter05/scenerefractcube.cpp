#include "scenerefractcube.h"
#include "texture.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
using glm::vec3;
using glm::mat4;

SceneRefractCube::SceneRefractCube() : angle(0.0f), tPrev(0.0f), rotSpeed(glm::pi<float>() / 8.0f),
                                       teapot(14, glm::mat4(1.0f)){ }

void SceneRefractCube::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);
    angle = glm::radians(90.0f);

	GLuint cubeTex = Texture::loadHdrCubeMap("../media/texture/cube/pisa-hdr-512/pisa");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);


#ifdef __APPLE__
	prog.setUniform("CubeMapTex", 0);
#endif
}

void SceneRefractCube::update( float t )
{
  float deltaT = t - tPrev;
  if(tPrev == 0.0f) deltaT = 0.0f;
  tPrev = t;

  angle += rotSpeed * deltaT;
  if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
}

void SceneRefractCube::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cameraPos = vec3( 7.0f * cos(angle), 2.0f, 7.0f * sin(angle));
    view = glm::lookAt(cameraPos, vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));

	// Draw sky
	skyProg.use();
	model = mat4(1.0f);
	setMatrices(skyProg);
	sky.render();

	// Draw teapot
	prog.use();
    prog.setUniform("WorldCameraPosition", cameraPos);
    prog.setUniform("Material.Eta", 0.94f);
    prog.setUniform("Material.ReflectionFactor", 0.1f);

    model = glm::translate(mat4(1.0f), vec3(0.0f,-1.0f,0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f,0.0f,0.0f));
    setMatrices(prog);
    teapot.render();
}

void SceneRefractCube::setMatrices(GLSLProgram &p)
{
    mat4 mv = view * model;
    p.setUniform("ModelMatrix", model);
    p.setUniform("MVP", projection * mv);
}

void SceneRefractCube::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(50.0f), (float)w/h, 0.3f, 100.0f);
}

void SceneRefractCube::compileAndLinkShader()
{
  try {
#ifdef __APPLE__
    prog.compileShader("shader/cubemap_refract_41.vs");
    prog.compileShader("shader/cubemap_refract_41.fs");
#else
    prog.compileShader("shader/cubemap_refract.vs");
    prog.compileShader("shader/cubemap_refract.fs");

	skyProg.compileShader("shader/skybox.vert.glsl");
	skyProg.compileShader("shader/skybox.frag.glsl");
	skyProg.link();
#endif
    prog.link();
    prog.use();
  } catch(GLSLProgramException & e) {
    cerr << e.what() << endl;
    exit( EXIT_FAILURE );
  }
}
