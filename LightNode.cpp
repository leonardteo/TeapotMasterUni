#include "LightNode.h"


LightNode::LightNode(string id, GLenum lightId, LightType type) : Node(id)
{
	this->type = type;
	this->lightId = lightId;
	this->on = true;

	this->debug = true;
	
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
		//By default directional light shines from z=-1
		this->position[0] = 0.0f;
		this->position[1] = 0.0f;
		this->position[2] = -1.0f;
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

	this->intensity = 1.0f;
}


void LightNode::render(enum RenderType renderType)
{

	//Turn this on if it's on
	if (this->on)
	{	
		glEnable(this->lightId);
	} else {
		glDisable(this->lightId);
	}	
	
	//Do transformations
	glPushMatrix();
		
		//Model transform
		this->modelTransform();
	
		//Do lighting
		
		//Calculate all color values based on intensity
		GLfloat ambient[4];
		GLfloat diffuse[4];
		GLfloat specular[4];
		for (int i=0; i<4; i++)
		{
			ambient[i] = this->intensity * this->ambient[i];
			diffuse[i] = this->intensity * this->diffuse[i];
			specular[i] = this->intensity * this->specular[i];
		}
		
		glLightfv(this->lightId, GL_AMBIENT, ambient);
		glLightfv(this->lightId, GL_DIFFUSE, diffuse);
		glLightfv(this->lightId, GL_SPECULAR, specular);
		glLightfv(this->lightId, GL_POSITION, this->position);

		if (this->type == SPOTLIGHT)
		{
			glLightf(this->lightId, GL_SPOT_CUTOFF, this->spotCutOff);
			glLightfv(this->lightId, GL_SPOT_DIRECTION, this->spotDirection);
		}
		
		//If debug, show the position of the light
		if (this->debug)
		{
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			
			glColor3f(this->diffuse[0], this->diffuse[1], this->diffuse[2]);
			
			if (this->type == POINTLIGHT)
			{
				glutSolidSphere(1.0f, 8, 8);
			}
			if (this->type == SPOTLIGHT)
			{
				glPushMatrix();
					glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
					glutWireCone(1.0f, 2.0f, 10, 1);		
				glPopMatrix();
			}
			if (this->type == DIRECTIONAL)
			{
				glBegin(GL_LINES);
					glVertex3f(0.0f, 0.0f, 0.0f);
					glVertex3f(0.0f, 0.0f, 1.0f);
					glVertex3f(0.1f, 0.1f, 0.0f);
					glVertex3f(0.1f, 0.1f, 1.0f);
					glVertex3f(-0.1f, -0.1f, 0.0f);
					glVertex3f(-0.1f, -0.1f, 1.0f);
				glEnd();	
				glPushMatrix();
				glTranslatef(0.0f, 0.0f, 1.0f);
				glutWireCone(0.2f, 1.0f, 5, 1);	
				glPopMatrix();
				
			}
			
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_LIGHTING);
		}
		

		//Draw children
		for (list<Node*>::iterator child = this->children->begin(); child != this->children->end(); child++)
		{
			(*child)->render(renderType);
		}


	glPopMatrix();	

}


LightNode::~LightNode(void)
{
}
