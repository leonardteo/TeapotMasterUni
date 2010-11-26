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
#include "Vector3.h"
#include "Matrix4.h"

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
	TRANSLATE, SCALE, ROTATE, BALLROTATE
};	

class TransformNode : public Node 
{
	
public:
	//Data members
	TransformType transformType;
	Vector3 ballAxis;
	float ballAngleDisplacement;
	Matrix4 ballRotationMatrix;
	
	//Methods
	TransformNode(string id = "", enum TransformType = TRANSLATE);
	~TransformNode();
	void rotateBall(Vector3 axis, float angle);
	void render(RenderType renderType = ALL_OBJECTS);
	
};

#endif
