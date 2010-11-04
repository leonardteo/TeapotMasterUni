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

#include <list>

#include "Node.h"

//OpenGL libraries
#ifdef __APPLE__
#include <GLEW/GLEW.h>
#include <GLUT/GLUT.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

using namespace std;

enum TransformType {
	TRANSLATE, SCALE, ROTATE
};	

class TransformNode : public Node 
{
	
public:
	//Data members
	TransformType transformType;
	
	//Methods
	TransformNode(string id = "", enum TransformType = TRANSLATE);
	~TransformNode();
	void render(RenderType renderType = ALL_OBJECTS);
	
};

#endif
