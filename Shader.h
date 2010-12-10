#ifndef SHADER_H
#define SHADER_H

/**
Shader Class
**/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//OpenGL libraries
#ifdef __APPLE__
#include <GLEW/GLEW.h>
#include <GLUT/GLUT.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

using namespace std;

class Shader
{
public:

	//Data members
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint shaderProgram;

	//Shader source
	char* vertexShaderSource;
	char* fragmentShaderSource;

	//Constructor/Destructor
	Shader(void);
	~Shader(void);

	//Load in shaders
	void loadVertexShader(char* filename);
	void loadFragmentShader(char* filename);

	//Compilation and linking
	bool compile();

	//Uniforms and varying shader vars
	GLint modelViewMatrix;
	GLint projectionMatrix;
	GLint shadowMap;
	GLint vertices;

private:
	char* readTextFile(char* filename);
};

#endif