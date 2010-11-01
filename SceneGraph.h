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

#ifdef __APPLE__
#include <GLEW/GLEW.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>	
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
	
	Node* getNode(string nodeName);
	
};

#endif;