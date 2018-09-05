#include "sceneseparable.h"

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using std::string;
#include <vector>
#include <iterator>

#include "glutils.h"

void SceneSeparable::initScene()
{
	// ********************************************************
	// Create the separable shader programs.  Call one of these.
	// **********************************************************
	createShaderPrograms1();
	//createShaderPrograms2();

	// **********************************************************
	// Create the two shader pipelines
	// ********************************************************

	// Create two pipelines
	glCreateProgramPipelines(2, pipelines);
	// Pipeline 1 uses the vertex shader (programs[0]) and first fragment shader (programs[1])
	glUseProgramStages(pipelines[0], GL_VERTEX_SHADER_BIT, programs[0]);
	glUseProgramStages(pipelines[0], GL_FRAGMENT_SHADER_BIT, programs[1]);
	// Pipeline 2 uses the vertex shader (programs[0]) and second fragment shader (programs[2])
	glUseProgramStages(pipelines[1], GL_VERTEX_SHADER_BIT, programs[0]);
	glUseProgramStages(pipelines[1], GL_FRAGMENT_SHADER_BIT, programs[2]);

	setupVao();
}

SceneSeparable::SceneSeparable() { }

// Compiles the shaders, creates the shader programs and the two program pipelines.
// This version uses glCreateShaderProgramv to create the stages.
void SceneSeparable::createShaderPrograms1() {
	std::string vertCode = loadShaderCode("shader/separable.vert.glsl");
	std::string fragCode1 = loadShaderCode("shader/separable1.frag.glsl");
	std::string fragCode2 = loadShaderCode("shader/separable2.frag.glsl");

	const GLchar * codePtrs[] = { vertCode.c_str(), fragCode1.c_str(), fragCode2.c_str() };

	programs[0] = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, codePtrs);
	checkLinkStatus(programs[0]);
	programs[1] = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, codePtrs + 1);
	checkLinkStatus(programs[1]);
	programs[2] = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, codePtrs + 2);
	checkLinkStatus(programs[2]);
}

// Compiles the shaders, creates the shader programs and the two program pipelines.
// This version uses glCreateProgram to create the shader programs.
void SceneSeparable::createShaderPrograms2() {
	GLuint vertShader = compileShader("shader/separable.vert.glsl", GL_VERTEX_SHADER);
	GLuint fragShader1 = compileShader("shader/separable1.frag.glsl", GL_FRAGMENT_SHADER);
	GLuint fragShader2 = compileShader("shader/separable2.frag.glsl", GL_FRAGMENT_SHADER);

	programs[0] = glCreateProgram();
	programs[1] = glCreateProgram();
	programs[2] = glCreateProgram();

	glProgramParameteri(programs[0], GL_PROGRAM_SEPARABLE, GL_TRUE);
	glProgramParameteri(programs[1], GL_PROGRAM_SEPARABLE, GL_TRUE);
	glProgramParameteri(programs[2], GL_PROGRAM_SEPARABLE, GL_TRUE);

	// Attach the shaders to the program objects
	glAttachShader(programs[0], vertShader);
	glAttachShader(programs[1], fragShader1);
	glAttachShader(programs[2], fragShader2);

	link(programs[0]);
	link(programs[1]);
	link(programs[2]);
}

void SceneSeparable::setupVao() {
	/////////////////// Create the VBO ////////////////////
	float positionData[] = {
		-0.8f, -0.8f, 0.0f,
		0.8f, -0.8f, 0.0f,
		0.0f,  0.8f, 0.0f };
	float colorData[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f };


	// Create and populate the buffer objects
	GLuint vboHandles[2];
	glGenBuffers(2, vboHandles);
	GLuint positionBufferHandle = vboHandles[0];
	GLuint colorBufferHandle = vboHandles[1];

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);

	// Create and set-up the vertex array object
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	glEnableVertexAttribArray(0);  // Vertex position
	glEnableVertexAttribArray(1);  // Vertex color

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
}

std::string SceneSeparable::loadShaderCode(const std::string & fName) {

	// Load contents of file
	std::ifstream inFile(fName);
	if (!inFile) {
		std::cerr << "Error opening file: " << fName << std::endl;
		exit(EXIT_FAILURE);
	}

	std::stringstream code;
	code << inFile.rdbuf();
	inFile.close();
	std::string codeStr(code.str());

	return codeStr;
}

GLuint SceneSeparable::compileShader( const std::string & fName, GLenum shaderType ) {

	std::string codeStr = loadShaderCode(fName);
    
    // Create the shader object
    GLuint shader = glCreateShader(shaderType);
    if (0 == shader) {
        std::cerr << "Error creating shader." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Load the source code into the shader object
    const GLchar* codeArray[] = { codeStr.c_str() };
    glShaderSource(shader, 1, codeArray, NULL);

    // Compile the shader
    glCompileShader(shader);

    // Check compilation status
    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (GL_FALSE == result) {
        std::cerr << "Shader compilation failed for " << fName << std::endl;
        std::cerr << getShaderInfoLog(shader) << std::endl;
        exit(EXIT_FAILURE);
    }

    return shader;
}

void SceneSeparable::link(GLuint program)
{
    // Link the program
    glLinkProgram( program );
	checkLinkStatus(program);
}

void SceneSeparable::checkLinkStatus(GLuint program) {
	// Check for successful linking
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (GL_FALSE == status) {
		std::cerr << "Failed to link shader program!" << std::endl;
		std::cerr << getProgramInfoLog(program) << std::endl;
		exit(EXIT_FAILURE);
	}
}

std::string SceneSeparable::getShaderInfoLog(GLuint shader) {
    GLint logLen;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);

    std::string log;
    if (logLen > 0) {
        log.resize(logLen, ' ');
        GLsizei written;
        glGetShaderInfoLog(shader, logLen, &written, &log[0]);
    }

    return log;
}

std::string SceneSeparable::getProgramInfoLog(GLuint program) {
    GLint logLen;
    glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLen );

    std::string log;
    if (logLen > 0) {
        log.resize(logLen, ' ');
        GLsizei written;
        glGetProgramInfoLog(program, logLen, &written, &log[0]);
    } 
    return log;
}

void SceneSeparable::update( float t )
{

}

void SceneSeparable::render()
{
    glUseProgram(0);

	// Set the uniform
	uniLocation = glGetUniformLocation(programs[0], "uColorMask");
	glProgramUniform3f(programs[0], uniLocation, 0.0f, 1.0f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vaoHandle);

	// Use the first pipeline on the left
	glViewport(0, 0, width / 2, height);
    glBindProgramPipeline(pipelines[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3 );

	// Use the second pipleine on the right
	glViewport(width / 2, 0, width / 2, height);
	glBindProgramPipeline(pipelines[1]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(0);
}

void SceneSeparable::resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);
}
