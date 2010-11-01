/*
 *  Node.cpp
 *  OBJViewer
 *
 *  Created by Leonard Teo on 10-10-05.
 *
 */

#include "Node.h"

//Constructor
Node::Node()
{
	this->parent = NULL;
	this->children = new vector<Node*>;
	this->scale = new Vector3(1.0f, 1.0f, 1.0f);
	this->rotate = new Vector3();
	this->translate = new Vector3();
	this->id = "";			//Blank ID?
}

//Destructor
Node::~Node()
{
	this->children->clear();	//Clear the vector
	delete this->children;
	delete this->scale;
	delete this->rotate;
	delete this->translate;
	delete this;
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

//Abstract method for rendering - must be overloaded by child classes
void Node::render(RenderType renderType)
{	
	for (int i=0; i<this->children->size(); i++)
	{
		this->children->at(i)->render(renderType);
	}
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
