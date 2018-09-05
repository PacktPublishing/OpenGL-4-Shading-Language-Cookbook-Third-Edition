#pragma once

#include "scene.h"

#include "cookbookogl.h"
#include <string>

class SceneSeparable : public Scene
{
private:
    GLuint vaoHandle;
    GLuint programs[3];
    GLuint pipelines[2];
    GLint uniLocation;

    void link(GLuint);
	void checkLinkStatus(GLuint program);
    GLuint compileShader( const std::string & fName, GLenum shaderType );

    std::string getShaderInfoLog(GLuint shader);
    std::string getProgramInfoLog(GLuint program);

public:
    SceneSeparable();

	// Create the shader programs using glCreateShaderProgramv
	void createShaderPrograms1();
	// Create the shader programs using glCreateProgram
	void createShaderPrograms2();

	void setupVao();

    void initScene();
	std::string loadShaderCode(const std::string & fName);
    void update( float t );
    void render();
    void resize(int, int);
};
