#include "sceneparticles.h"
#include "texture.h"
#include "particleutils.h"

#include <random>
#include <iostream>
using std::endl;
using std::cerr;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;

SceneParticles::SceneParticles() : time(0), particleLifetime(5.5f), nParticles(8000),
                                   emitterPos(1,0,0), emitterDir(-1,2,0)
{ }

void SceneParticles::initScene()
{
    compileAndLinkShader();

    glClearColor(0.1f,0.1f,0.1f,1.0f);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

	angle = glm::half_pi<float>();
	//angle= 0;
	initBuffers();

    // The particle texture
    glActiveTexture(GL_TEXTURE0);
    Texture::loadTexture("../media/texture/bluewater.png");

    prog.use();
    prog.setUniform("ParticleTex", 0);
    prog.setUniform("ParticleLifetime", particleLifetime);
    prog.setUniform("ParticleSize", 0.05f);
    prog.setUniform("Gravity", vec3(0.0f,-0.2f,0.0f));
    prog.setUniform("EmitterPos", emitterPos);

    flatProg.use();
    flatProg.setUniform("Color", glm::vec4(0.4f,0.4f,0.4f,1.0f));
}


void SceneParticles::initBuffers()
{
    // Generate the buffers for initial velocity and start (birth) time
    glGenBuffers(1, &initVel);   // Initial velocity buffer
    glGenBuffers(1, &startTime); // Start time buffer

    // Allocate space for all buffers
    int size = nParticles * sizeof(float);
    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glBufferData(GL_ARRAY_BUFFER, size * 3, 0, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, startTime);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);

    // Fill the first velocity buffer with random velocities
    glm::mat3 emitterBasis = ParticleUtils::makeArbitraryBasis(emitterDir);
    vec3 v(0.0f);
    float velocity, theta, phi;
    std::vector<GLfloat> data(nParticles * 3);
    for (uint32_t i = 0; i < nParticles; i++ ) {

		theta = glm::mix(0.0f, glm::pi<float>() / 20.0f, randFloat());
		phi = glm::mix(0.0f, glm::two_pi<float>(), randFloat());

        v.x = sinf(theta) * cosf(phi);
        v.y = cosf(theta);
        v.z = sinf(theta) * sinf(phi);

        velocity = glm::mix(1.25f,1.5f,randFloat());
        v = glm::normalize(emitterBasis * v) * velocity;

        data[3*i]   = v.x;
        data[3*i+1] = v.y;
        data[3*i+2] = v.z;
    }
    glBindBuffer(GL_ARRAY_BUFFER,initVel);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size * 3, data.data());

    // Fill the start time buffer
    float rate = particleLifetime / nParticles;
    for( int i = 0; i < nParticles; i++ ) {
        data[i] = rate * i;
    }
    glBindBuffer(GL_ARRAY_BUFFER,startTime);
    glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data.data());

    glBindBuffer(GL_ARRAY_BUFFER,0);

    glGenVertexArrays(1, &particles);
	glBindVertexArray(particles);
    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, startTime);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glVertexAttribDivisor(0,1);
    glVertexAttribDivisor(1,1);

    glBindVertexArray(0);

}

float SceneParticles::randFloat() {
    return rand.nextFloat();
}

void SceneParticles::update( float t ) {
    time = t;
    angle = std::fmod(angle + 0.01f, glm::two_pi<float>());
}

void SceneParticles::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	view = glm::lookAt(vec3(3.0f * cos(angle),1.5f,3.0f * sin(angle)), vec3(0.0f,1.5f,0.0f), vec3(0.0f,1.0f,0.0f));
    model = mat4(1.0f);

	flatProg.use();
	setMatrices(flatProg);
	grid.render();

	glDepthMask(GL_FALSE);
    prog.use();
    setMatrices(prog);
    prog.setUniform("Time", time);
    glBindVertexArray(particles);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticles);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
}

void SceneParticles::setMatrices(GLSLProgram & p)
{
    mat4 mv = view * model;
    p.setUniform("MV", mv);
    p.setUniform("Proj", projection);
}

void SceneParticles::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(60.0f), (float)w/h, 0.3f, 100.0f);
}

void SceneParticles::compileAndLinkShader()
{
	try {
		prog.compileShader("shader/particles_frag.glsl");
		prog.compileShader("shader/particles_vert.glsl");

    	prog.link();
    	prog.use();

        flatProg.compileShader("shader/flat_frag.glsl");
        flatProg.compileShader("shader/flat_vert.glsl");

        flatProg.link();
    } catch(GLSLProgramException &e ) {
    	cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}
