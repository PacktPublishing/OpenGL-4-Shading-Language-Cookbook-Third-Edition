#include "scenebasic.h"

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

SceneBasic::SceneBasic() { }

void SceneBasic::initScene()
{
    // **************************************************************************************
    // Choose one of the following options for the shader program.
    //  1)  Compile the shader program normally
    //  2)  Load a binary (pre-compiled) shader program.  (file: "shader/program.bin")
    //  3)  Load a SPIR-V shader program. (files: "shader/vert.spv" and "shader/frag.spv")
    //
    // Optionally, you may attempt to write out the shader program binary using the function
    // writeShaderBinary().
    // **************************************************************************************

    // (1) Use this to load and compile the shader program.
    compileShaderProgram();

    // (2) Use this to load a binary shader.  Use the format provided when the binary was written.
    // int shaderFormat = 36385;
    // loadShaderBinary(shaderFormat);

    // (3) Load a SPIR-V shader
    // loadSpirvShader();

    // Optional:  use this to write the shader binary out to a file.
    //writeShaderBinary();

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
    glGenVertexArrays( 1, &vaoHandle );
    glBindVertexArray(vaoHandle);

    glEnableVertexAttribArray(0);  // Vertex position
    glEnableVertexAttribArray(1);  // Vertex color

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );
    glBindVertexArray(0);
}

void SceneBasic::loadSpirvShader() {

    std::cout << "Loading SPIR-V shaders." << std::endl;

    GLint status;
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);

    {
        std::cout <<  "Loading SPIR-V binary: shader/vert.spv" << std::endl;
        std::ifstream inStream("shader/vert.spv", std::ios::binary);
        std::istreambuf_iterator<char> startIt(inStream), endIt;
        std::vector<char> buffer(startIt, endIt);
        inStream.close();

        glShaderBinary(1, &vertShader, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, buffer.data(), buffer.size());
    }

    glSpecializeShaderARB( vertShader, "main", 0, 0, 0);

    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &status);
    if( GL_FALSE == status ) {
        std::cerr << "Failed to load vertex shader (SPIR-V)" << std::endl;
        std::cerr << getShaderInfoLog(vertShader) << std::endl;
        exit(-1);
    }

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    {
        std::cout <<  "Loading SPIR-V binary: shader/frag.spv" << std::endl;
        std::ifstream inStream("shader/frag.spv", std::ios::binary);
        std::istreambuf_iterator<char> startIt(inStream), endIt;
        std::vector<char> buffer(startIt, endIt);
        inStream.close();

        glShaderBinary(1, &fragShader, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, buffer.data(), buffer.size());
    }

    glSpecializeShaderARB( fragShader, "main", 0, 0, 0);

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
    if( GL_FALSE == status ) {
        std::cerr << "Failed to load fragment shader (SPIR-V)" << std::endl;
        std::cerr << getShaderInfoLog(fragShader) << std::endl;
        exit(-1);
    }

    // Create the program object
    programHandle = glCreateProgram();
    if (0 == programHandle) {
        std::cerr << "Error creating program object." << std::endl;
        exit(1);
    }

    glAttachShader(programHandle, vertShader);
    glAttachShader(programHandle, fragShader);
    glLinkProgram(programHandle);

    glGetProgramiv(programHandle, GL_LINK_STATUS, &status);
    if (GL_FALSE == status) {
        std::cerr << "Failed to link SPIR-V program" << std::endl;
        std::cerr << getProgramInfoLog(programHandle) << std::endl;
        exit(EXIT_FAILURE);
    }

    glUseProgram(programHandle);
}

void SceneBasic::compileShaderProgram() {
    std::cout << "Compiling shader program" << std::endl;

	//////////////////////////////////////////////////////
	/////////// Vertex shader //////////////////////////
	//////////////////////////////////////////////////////

	// Load contents of file
	std::ifstream inFile("shader/basic.vert.glsl");
	if (!inFile) {
		fprintf(stderr, "Error opening file: shader/basic.vert.glsl\n");
		exit(1);
	}

	std::stringstream code;
	code << inFile.rdbuf();
	inFile.close();
	string codeStr(code.str());

	// Create the shader object
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	if (0 == vertShader) {
		std::cerr << "Error creating vertex shader." << std::endl;
		exit(EXIT_FAILURE);
	}

	// Load the source code into the shader object
	const GLchar* codeArray[] = { codeStr.c_str() };
	glShaderSource(vertShader, 1, codeArray, NULL);

	// Compile the shader
	glCompileShader(vertShader);

	// Check compilation status
	GLint result;
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
	if (GL_FALSE == result) {
		std::cerr << "Vertex shader compilation failed!" << std::endl;
		std::cerr << getShaderInfoLog(vertShader) << std::endl;
        exit(EXIT_FAILURE);
	}

	//////////////////////////////////////////////////////
	/////////// Fragment shader //////////////////////////
	//////////////////////////////////////////////////////

	// Load contents of file into shaderCode here
	std::ifstream fragFile("shader/basic.frag.glsl");
	if (!fragFile) {
		fprintf(stderr, "Error opening file: shader/basic.frag.glsl\n");
		exit(1);
	}

	std::stringstream fragCode;
	fragCode << fragFile.rdbuf();
	fragFile.close();
	codeStr = fragCode.str();

	// Create the shader object
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (0 == fragShader) {
		fprintf(stderr, "Error creating fragment shader.\n");
		exit(1);
	}

	// Load the source code into the shader object
	codeArray[0] = codeStr.c_str();
	glShaderSource(fragShader, 1, codeArray, NULL);

	// Compile the shader
	glCompileShader(fragShader);

	// Check compilation status
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
	if (GL_FALSE == result) {
        std::cerr << "Fragment shader compilation failed!" << std::endl;
        std::cerr << getShaderInfoLog(vertShader) << std::endl;
        exit(EXIT_FAILURE);
	}

	linkMe(vertShader, fragShader);
}

void SceneBasic::loadShaderBinary(GLint format) {
    std::cout << "Loading shader binary: shader/program.bin (format = " << format << ")" << std::endl;

    // Create the program object
	programHandle = glCreateProgram();
	if (0 == programHandle) {
		std::cerr << "Error creating program object." << std::endl;
		exit(EXIT_FAILURE);
	}

	std::ifstream inStream("shader/program.bin", std::ios::binary);
	std::istreambuf_iterator<char> startIt(inStream), endIt;
	std::vector<char> buffer(startIt, endIt);
	inStream.close();
	glProgramBinary(programHandle, format, buffer.data(), buffer.size());

	// Check for successful linking
	GLint status;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &status);
	if (GL_FALSE == status) {
		std::cerr << "Failed to load binary shader program!" << std::endl;
        std::cerr << getProgramInfoLog(programHandle) << std::endl;
        exit(EXIT_FAILURE);
	}
	glUseProgram(programHandle);
}

void SceneBasic::linkMe(GLint vertShader, GLint fragShader)
{
    // Create the program object
    programHandle = glCreateProgram();
    if(0 == programHandle) {
        std::cerr << "Error creating program object." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Bind index 0 to the shader input variable "VertexPosition"
    //glBindAttribLocation(programHandle, 0, "VertexPosition");
    // Bind index 1 to the shader input variable "VertexColor"
    //glBindAttribLocation(programHandle, 1, "VertexColor");

    // Attach the shaders to the program object
    glAttachShader( programHandle, vertShader );
    glAttachShader( programHandle, fragShader );

    // Link the program
    glLinkProgram( programHandle );

    // Check for successful linking
    GLint status;
    glGetProgramiv( programHandle, GL_LINK_STATUS, &status );
    if (GL_FALSE == status) {
        std::cerr << "Failed to link shader program!" << std::endl;
        std::cerr << getProgramInfoLog(programHandle) << std::endl;
        exit(EXIT_FAILURE);
    }

	// Clean up shader objects
	glDetachShader(programHandle, vertShader);
	glDetachShader(programHandle, fragShader);
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

    glUseProgram( programHandle );
}

void SceneBasic::writeShaderBinary() {
    GLint formats;
    glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);
    std::cout << "Number of binary formats supported by this driver = " << formats << std::endl;

    if( formats > 0 ) {
        GLint length;
        glGetProgramiv(programHandle, GL_PROGRAM_BINARY_LENGTH, &length);
        std::cout << "Program binary length = " << length << std::endl;

        std::vector<GLubyte> buffer(length);
        GLenum format;
        glGetProgramBinary(programHandle, length, NULL, &format, buffer.data());
        std::string fName("shader/program.bin");

        std::cout << "Writing to " << fName << ", binary format = " << format << std::endl;
        std::ofstream out(fName.c_str(), std::ios::binary);
        out.write( reinterpret_cast<char *>(buffer.data()), length );
        out.close();
    } else {
        std::cout << "No binary formats supported by this driver.  Unable to write shader binary." << std::endl;
    }
}

std::string SceneBasic::getShaderInfoLog(GLuint shader) {
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

std::string SceneBasic::getProgramInfoLog(GLuint program) {
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

void SceneBasic::update( float t )
{

}

void SceneBasic::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vaoHandle);
    glDrawArrays(GL_TRIANGLES, 0, 3 );
    glBindVertexArray(0);
}

void SceneBasic::resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);
}
