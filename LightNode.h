#ifndef LIGHTNODE_H
#define LIGHTNODE_H

#include "Node.h"
#include <list>

//OpenGL libraries
#ifdef __APPLE__
#include <GLEW/GLEW.h>
#include <GLUT/GLUT.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

using namespace std;

enum LightType{
	SPOTLIGHT, DIRECTIONAL, POINTLIGHT
};

class LightNode : public Node
{
public:
	LightNode(string id = "", GLenum lightId = GL_LIGHT0, LightType = POINTLIGHT);
	~LightNode(void);

	LightType type;
	GLenum lightId;	

	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat position[4];
	GLfloat spotDirection[3];
	GLfloat intensity;

	GLfloat spotCutOff;

	bool on;

	void render(RenderType renderType = ALL_OBJECTS);
	
private:
	bool debug;

};

#endif