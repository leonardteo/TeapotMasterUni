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
	for (list<Node*>::iterator child = this->children->begin(); child != this->children->end(); child++)
	{
		(*child)->render(renderType);
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

/**
 Recursive depth first search
 **/
Node* Node::getNode(string str)
{
	cout << "Inside node: " << this->id << ". ";
	
	cout << "Comparing '" << str << "' to '" << this->id << "'. Result: " << str.compare(this->id) << endl;
	
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
