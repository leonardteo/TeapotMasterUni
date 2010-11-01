#ifndef LIGHTNODE_H
#define LIGHTNODE_H

#include "Node.h"

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

enum LightType{
	SPOTLIGHT, DIRECTIONAL, POINTLIGHT
};

class LightNode : public Node
{
public:
	LightNode(GLenum id, LightType type);
	~LightNode(void);

	LightType type;
	GLenum id;	

	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat position[4];
	GLfloat spotDirection[3];

	GLfloat spotCutOff;

	bool on;

	void render(RenderType renderType = ALL_OBJECTS);

};

#endif