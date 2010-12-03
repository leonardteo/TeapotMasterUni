/*
 *  Node.cpp
 *  OBJViewer
 *
 *  Created by Leonard Teo on 10-10-05.
 *
 */

#include "Node.h"

void Node::init()
{
	this->parent = NULL;
	this->shader = NULL;
	this->children = new list<Node*>;
	this->scale = new Vector3(1.0f, 1.0f, 1.0f);
	this->rotate = new Vector3();
	this->translate = new Vector3();	
}

//Constructor
Node::Node()
{
	this->init();
	this->id = "";			//Blank ID
}

Node::Node(string id)
{
	this->init();
	this->id = id;
}

//Destructor
Node::~Node()
{
	this->children->clear();	//Clear the list
	delete this->children;
	delete this->scale;
	delete this->rotate;
	delete this->translate;
	delete this;
}

//Attach a shader for rendering the node
void Node::attachShader(Shader* shader)
{
	this->shader = shader;
}

//Add a child
void Node::addChild(Node* child)
{
	//Set the child node's parent as this one
	child->setParent(this);
	
	//Add the child node
	this->children->push_back(child);

}

//Set the parent
void Node::setParent(Node* parent)
{
	this->parent = parent;
}

//Abstract method for doing model transformation
void Node::modelTransform()
{
	glTranslatef(this->translate->x, this->translate->y, this->translate->z);
	
	//This piece of code is not good because the order of rotation is actually important. 
	//We need to figure out a way to rotate an entire local coordinate system.
	//This should be relatively simple but we'll need to figure it out... 
	glRotatef(this->rotate->z, 0.0f, 0.0f, 1.0f);
	glRotatef(this->rotate->y, 0.0f, 1.0f, 0.0f);
	glRotatef(this->rotate->x, 1.0f, 0.0f, 0.0f);
	
	glScalef(this->scale->x, this->scale->y, this->scale->z);
}

//Abstract method for rendering - must be overloaded by child classes
void Node::render(RenderType renderType)
{	
	glPushMatrix();
	
	//Model transform
	this->modelTransform();
	
	for (list<Node*>::iterator child = this->children->begin(); child != this->children->end(); child++)
	{
		(*child)->render(renderType);
	}
	
	glPopMatrix();
}

//Abstract method for doing view transformations
void Node::viewTransform()
{
	//Transform view according to local node orientation
	glRotatef(-this->rotate->z, 0.0f, 0.0f, 1.0f);	//Roll
	glRotatef(-this->rotate->y, 0.0f, 1.0f, 0.0f);	//Heading	
	glRotatef(-this->rotate->x, 1.0f, 0.0f, 0.0f);	//Pitch
	
	glTranslatef(-this->translate->x, -this->translate->y, -this->translate->z);

	if (this->parent != NULL)
	{
		this->parent->viewTransform();
	}
}

void Node::setTranslation(float x, float y, float z)
{
	this->translate->x = x;
	this->translate->y = y; 
	this->translate->z = z;
}

/**
 Recursive depth first search
 **/
Node* Node::getNode(string str)
{
	if (str.compare(this->id) == 0)
	{
		return this;
	} else {
		for (list<Node*>::iterator child = this->children->begin(); child != this->children->end(); child++)
		{
			Node* node = (*child)->getNode(str);
			if (node != NULL)
				return node;
		}

	}
	
	return NULL;
}


/**
 Function for getting the object space position of any node that's not a camera
 Traverse up the tree until we're on the root node, then do a modeltransform all the way back down
 **/
Vector3 Node::getPosition()
{
	stack<Node*> nodes;	//Stack of node pointers
	
	//Upward traversal
	Node* currentNode = this;
	while (currentNode->parent != NULL)
	{
		nodes.push(currentNode);
		currentNode = currentNode->parent;
	}
	
	//For each element do a model transformation
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();			//Remember the current modelview matrix just in case
	glLoadIdentity();		//Clear the modelview matrix
	
	while (!nodes.empty())
	{
		currentNode = nodes.top();
		currentNode->modelTransform();
		nodes.pop();
	}
	
	//Get the matrix
	float m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	
	glPopMatrix();
	
	Vector3 returnVec(m[12], m[13], m[14]);
	return returnVec;
}