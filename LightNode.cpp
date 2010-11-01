#include "LightNode.h"


LightNode::LightNode(GLenum id, LightType type) : Node()
{
	this->type = type;
	this->id = id;
	this->on = true;

	this->ambient[0] = 1.0f;
	this->ambient[1] = 1.0f;
	this->ambient[2] = 1.0f;
	this->ambient[3] = 1.0f;

	this->diffuse[0] = 1.0f;
	this->diffuse[1] = 1.0f;
	this->diffuse[2] = 1.0f;
	this->diffuse[3] = 1.0f;

	this->specular[0] = 1.0f;
	this->specular[1] = 1.0f;
	this->specular[2] = 1.0f;
	this->specular[3] = 1.0f;

	if (this->type == DIRECTIONAL)
	{
		//By default directional light shines from z=1
		this->position[0] = 0.0f;
		this->position[1] = 0.0f;
		this->position[2] = 1.0f;
		this->position[3] = 0.0f;

		
	} else {
		//By default this light is at 0
		this->position[0] = 0.0f;
		this->position[1] = 0.0f;
		this->position[2] = 0.0f;
		this->position[3] = 1.0f;
	}

	//By default the light is pointing at positive z (sounds retarded but this is consistent with all the 3D models)
	this->spotDirection[0] = 0.0f;
	this->spotDirection[1] = 0.0f;
	this->spotDirection[2] = 1.0f; 

	this->spotCutOff = 45.0f;

}


void LightNode::render(enum RenderType renderType)
{

	//Do transformations
	glPushMatrix();
		glTranslatef(this->translate->x, this->translate->y, this->translate->z);

		//This piece of code is not good because the order of rotation is actually important. 
		//We need to figure out a way to rotate an entire local coordinate system.
		//This should be relatively simple but we'll need to figure it out... 
		glRotatef(this->rotate->z, 0.0f, 0.0f, 1.0f);
		glRotatef(this->rotate->y, 0.0f, 1.0f, 0.0f);
		glRotatef(this->rotate->x, 1.0f, 0.0f, 0.0f);

		glScalef(this->scale->x, this->scale->y, this->scale->z);
		
			//Do lighting
			glLightfv(this->id, GL_AMBIENT, this->ambient);
			glLightfv(this->id, GL_DIFFUSE, this->diffuse);
			glLightfv(this->id, GL_SPECULAR, this->specular);
			glLightfv(this->id, GL_POSITION, this->position);

			if (this->type == SPOTLIGHT)
			{
				glLightf(this->id, GL_SPOT_CUTOFF, this->spotCutOff);
				glLightfv(this->id, GL_SPOT_DIRECTION, this->spotDirection);
			}

			if (this->on)
			{
				glEnable(this->id);
			} else {
				glDisable(this->id);
			}

		//Draw children
		for (int i=0; i < this->children->size(); i++)
		{
			this->children->at(i)->render(renderType);
		}

	glPopMatrix();	

}


LightNode::~LightNode(void)
{
}
