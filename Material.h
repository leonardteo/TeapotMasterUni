#ifndef MATERIAL_H
#define MATERIAL_H

//OpenGL libraries
#ifdef __APPLE__
#include <GLEW/GLEW.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>	
#endif

class Material
{
public:
	Material(void);
	~Material(void);

	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess[1];

	void setAmbient(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha = 1.0f);
	void setDiffuse(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha = 1.0f);
	void setSpecular(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha = 1.0f);
	void setDiffuseAndAmbient(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha = 1.0f);
	void setShininess(GLfloat power);
};

#endif