/*
 *  Node.h
 *  OBJViewer
 *
 *  Created by Leonard Teo on 10-10-05.
 *
 */

#ifndef NODE_H
#define NODE_H

#include <list>
#include <stack>
#include <string>

//OpenGL libraries
#ifdef __APPLE__
#include <GLEW/GLEW.h>
#include <GLUT/GLUT.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "Vector3.h"
#include "Shader.h"

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
	list<Node*>* children;	//Child nodes
	Node* parent;				//The node's parent
	string id;					//The node's ID so that we can search for a node

	//Shader
	Shader* shader;
	
	//Local Transforms
	Vector3* scale;				//Local scaling	
	Vector3* rotate;			//Local rotation on x, y, z axes
	Vector3* translate;			//Local translation

	//Methods
	Node();
	Node(string id);
	~Node();
	void addChild(Node*);
	void setParent(Node*);
	void setTranslation(float x, float y, float z);
	virtual void render(RenderType renderType = ALL_OBJECTS);
	virtual void viewTransform();
	Node* getNode(string str);
	Vector3 getPosition();
	void modelTransform();
	void attachShader(Shader* shader);
	
private:
	void init();
	
};

#endif
