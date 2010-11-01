/*
 *  Camera.h
 *  OBJViewer
 *
 *  Created by Leonard Teo on 10-10-06.
 *  Copyright 2010 Leonard Teo. All rights reserved.
 *
 */

#ifndef CAMERANODE_H
#define CAMERANODE_H

#include "Node.h"

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

enum CameraProjection {
	PERSPECTIVE, ORTHO
};

enum CameraType {
	POLAR, FIRSTPERSON
};


/**
 * Camera class extends Node
 */

class CameraNode : public Node 
{
public:
	
	//Data Members
	CameraProjection cameraProjection;
	CameraType cameraType;
	
	//Constructor/Destructor
	CameraNode(CameraType);
	~CameraNode();

	//@todo: Should these be private?

	//Members
	//Third person camera / polar camera
	float twist;				//Roll
	float elevation;			//Pitch
	float azimuth;				//Heading
	float distance;				//Distance from point of interest (assume local origin)
	float panX;					//Used for panning the camera
	float panY;

	//Orthographic camera
	float orthoZoom;			//The orthographic zoom factor

	bool perspectiveMode;		//Switch between perspective and ortho

	//Third person offsets - used for controlling mouse movements where you don't want to write directly to the twist, elevation, azimuth
	float twist_offset;
	float elevation_offset;
	float azimuth_offset;
	float distance_offset;
	float panX_offset;
	float panY_offset;
	float orthoZoom_offset;		//Used for handling mouse in and out

	//Projection settings
	float fov;					//Field of View
	float nearPlane;			//Near clipping plane
	float farPlane;			//Far clipping plane

	//Methods
	void setProjection(int width, int height);
	void viewTransform();
};

#endif
