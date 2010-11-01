/*
 *  Node.h
 *  OBJViewer
 *
 *  Created by Leonard Teo on 10-10-05.
 *
 */

#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>

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

#include "Vector3.h"

using namespace std;

enum NodeType {
	POLYMESH, CAMERA, LIGHT, TRANSFORM
};

enum RenderType {
	OPAQUE_OBJECTS, TRANSPARENT_OBJECTS, ALL_OBJECTS
};

class Node
{
public:

	//Data Members
	NodeType type;				//The type of node this is
	vector<Node*>* children;	//Child nodes
	Node* parent;				//The node's parent
	string id;					//The node's ID so that we can search for a node
	
	//Local Transforms
	Vector3* scale;				//Local scaling	
	Vector3* rotate;			//Local rotation on x, y, z axes
	Vector3* translate;			//Local translation

	//Methods
	Node();
	~Node();
	void addChild(Node*);
	void setParent(Node*);
	void setTranslation(float x, float y, float z);
	virtual void render(RenderType renderType = ALL_OBJECTS);
	virtual void viewTransform();
	
};

#endif
