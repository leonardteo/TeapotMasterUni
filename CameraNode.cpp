/*
 *  Camera.cpp
 *  OBJViewer
 *
 *  Created by Leonard Teo on 10-10-06.
 *  Copyright 2010 Leonard Teo. All rights reserved.
 *
 */

#include "CameraNode.h"

CameraNode::CameraNode(string id, CameraType cameraType) : Node(id)
{
	//Set node type
	this->type = CAMERA;
	this->cameraType = cameraType;

	//Polar camera settings
	this->azimuth = 0.0;
	this->elevation = 0.0;
	this->twist = 0.0;
	this->distance = 0.0;
	this->panX = 0.0;
	this->panY = 0.0;

	this->azimuth_offset = 0.0;
	this->elevation_offset = 0.0;
	this->twist_offset = 0.0;
	this->distance_offset = 0.0;
	this->panX_offset = 0.0;
	this->panY_offset = 0.0;
	this->orthoZoom_offset = 0.0f;

	//Default Projection settings
	this->nearPlane = 0.1f;
	this->farPlane = 1000.0f;
	this->fov = 45;
	this->perspectiveMode = true;

	this->orthoZoom = 10;

}

CameraNode::~CameraNode()
{
	delete this;
}

/**
Sets up a projection for the camera
Must provide viewport width and height
**/
void CameraNode::setProjection(int width, int height)
{
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Perspective mode or ortho
	float aspect_ratio = (float)width / (float)height;

	if (this->perspectiveMode){
		//If perspective mode, set up a perspective projection
		gluPerspective(this->fov, GLfloat(aspect_ratio), this->nearPlane, this->farPlane);

	} else {
		//If orthographic mode, set up an orthographic projection

		//Keep aspect ratio
		float left, right, bottom, top;

		if (aspect_ratio < 1.0){
			left = - (this->orthoZoom + this->orthoZoom_offset);
			right = (this->orthoZoom + this->orthoZoom_offset);
			bottom = -(this->orthoZoom + this->orthoZoom_offset) * ( 1.0f / aspect_ratio );
			top = (this->orthoZoom + this->orthoZoom_offset) * ( 1.0f / aspect_ratio );
		} else {
			left = -(this->orthoZoom + this->orthoZoom_offset) * aspect_ratio;
			right = (this->orthoZoom + this->orthoZoom_offset) * aspect_ratio;
			bottom = -(this->orthoZoom + this->orthoZoom_offset);
			top = (this->orthoZoom + this->orthoZoom_offset);
		}
		glOrtho(left, right, bottom, top, -this->farPlane, this->farPlane);
	}
}

/**
View transformations
**/
void CameraNode::viewTransform()
{
	
	//If first person
	if (this->cameraType == FIRSTPERSON){
		//To do: Make this rotate on a local, orthonormal basis
		glRotatef(this->rotate->z, 0.0f, 0.0f, 1.0f);	//Roll
		glRotatef(this->rotate->y, 0.0f, 1.0f, 0.0f);	//Heading
		glRotatef(this->rotate->x, 1.0f, 0.0f, 0.0f);	//Pitch
		glTranslatef(-this->translate->x, -this->translate->y, -this->translate->z);

	} else
	{
		//If third person

		//Do third person view transform
		//glTranslatef(-((GLfloat)this->panX + (GLfloat)this->panX_offset), -((GLfloat)this->panY + (GLfloat)this->panY_offset), -( (GLfloat)this->distance + (GLfloat)this->distance_offset) );		//Translate along the z axis away from camera
		glTranslatef(-(this->panX + this->panX_offset), -(this->panY + this->panY_offset), -( this->distance + this->distance_offset) );		//Translate along the z axis away from camera
		glRotatef(this->twist + this->twist_offset, 0.0f, 0.0f, 1.0f);							//Rotate around z axis (usually by 0)
		glRotatef(this->elevation + this->elevation_offset, 1.0f, 0.0f, 0.0f);					//Rotate around x axis
		glRotatef(this->azimuth + this->azimuth_offset, 0.0f, 1.0f, 0.0f);						//Rotate around y axis

		//Local view transform based on position/rotation of the node
		glRotatef(this->rotate->z, 0.0f, 0.0f, 1.0f);	//Roll
		glRotatef(this->rotate->y, 0.0f, 1.0f, 0.0f);	//Heading
		glRotatef(this->rotate->x, 1.0f, 0.0f, 0.0f);	//Pitch
		glTranslatef(-this->translate->x, -this->translate->y, -this->translate->z);
	}

	//Call parent viewTransform
	if (this->parent != NULL)
	{
		this->parent->viewTransform();
	}

}

/**
 Get the camera's global position
 **/
Vector3 CameraNode::getPosition()
{
	glMatrixMode(GL_MODELVIEW);
	
	glPushMatrix();
	glLoadIdentity();
	
	this->viewTransform();
	
	//Copy the current viewtransform to the modelview matrix4
	float m[16];
	glGetFloatv (GL_MODELVIEW_MATRIX, m);   
	
	glPopMatrix();
	
	//Invert the matrix
	Matrix4 matrix(m);
	Matrix4 inverseMatrix = matrix.inverse();
	
	Vector3 position(inverseMatrix.m[12], inverseMatrix.m[13], inverseMatrix.m[14]);
	return position;
	
	
}
