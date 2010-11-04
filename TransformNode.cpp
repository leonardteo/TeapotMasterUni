/*
 *  TransformNode.cpp
 *  Scene graph node for handling transformations
 *
 *  Created by Leonard Teo on 10-10-10.
 *  Copyright 2010 Leonard Teo. All rights reserved.
 *
 */

#include "TransformNode.h"

TransformNode::TransformNode(string id, TransformType transformType) : Node(id)
{
	this->type = TRANSFORM;
	this->transformType = transformType;
}

TransformNode::~TransformNode()
{
}

//Do all translations then render child nodes
void TransformNode::render(enum RenderType renderType)
{
	//Remember where we are
	glPushMatrix();
	
	switch (this->transformType)
	{
		case TRANSLATE:
			glTranslatef((GLfloat)this->translate->x, (GLfloat)this->translate->y, (GLfloat)this->translate->z);
			break;
		case ROTATE:
			glRotatef((GLfloat)this->rotate->x, (GLfloat)1.0f, (GLfloat)0.0f, (GLfloat)0.0f);
			glRotatef((GLfloat)this->rotate->y, (GLfloat)0.0f, (GLfloat)1.0f, (GLfloat)0.0f);
			glRotatef((GLfloat)this->rotate->z, (GLfloat)0.0f, (GLfloat)0.0f, (GLfloat)1.0f);
			break;
		case SCALE:
			glScalef((GLfloat)this->scale->x, (GLfloat)this->scale->y, (GLfloat)this->scale->z);
			break;
	}
	
	//Render children
	for (list<Node*>::iterator child = this->children->begin(); child != this->children->end(); child++)
	{
		(*child)->render();
	}

	
	glPopMatrix();	
	
}