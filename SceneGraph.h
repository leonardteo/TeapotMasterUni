/*
 *  SceneGraph.h
 *  TeapotMaster
 *
 *  Created by Leonard Teo on 10-10-31.
 *  Copyright 2010 Leonard Teo. All rights reserved.
 *
 *  Class for managing a scene graph
 *  
 */

#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

//OpenGL libraries
#ifdef __APPLE__
#include <GLEW/GLEW.h>
#include <GLUT/GLUT.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <string>

//Engine libraries
#include "Node.h"

class SceneGraph
{
public:
	//Data Members
	Node* rootNode;	

	//Methods
	SceneGraph(void);
	~SceneGraph(void);
	
	//Retrieve a node based on its name
	Node* getNode(string nodeName);
	
	void render(RenderType renderType = ALL_OBJECTS);
	
};

#endif;