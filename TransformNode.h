/*
 *  TransformNode
 *
 *	Scene Graph node for handling transformations
 *
 *  Copyright 2010 Leonard Teo. All rights reserved.
 *
 */

#ifndef TRANSFORMNODE_H
#define TRANSFORMNODE_H

#include "Node.h"

#ifdef __APPLE__
#include <GLEW/GLEW.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>	
#endif

enum TransformType {
	TRANSLATE, SCALE, ROTATE
};	

class TransformNode : public Node 
{
	
public:
	//Data members
	TransformType transformType;
	
	//Methods
	TransformNode(enum TransformType);
	~TransformNode();
	void render(RenderType renderType = ALL_OBJECTS);
	
};

#endif
