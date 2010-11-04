/*
 *  TeapotEngine.h
 *  Teapot Engine Class
 *
 *  Created by Leonard Teo on 10-10-31.
 *  Copyright 2010 Leonard Teo. All rights reserved.
 *
 */



#ifndef TEAPOTENGINE_H
#define TEAPOTENGINE_H

//OpenGL libraries
#ifdef __APPLE__
#include <GLEW/GLEW.h>
#include <GLUT/GLUT.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <stdlib.h>

#include "TeapotLevel.h"
#include "OBJModel.h"
#include "MyBitmap.h"
#include "Node.h"
#include "TransformNode.h"
#include "PolyMeshNode.h"
#include "Texture.h"
#include "Vector3.h"
#include "CameraNode.h"
#include "LightNode.h"


enum GameState {
	MENU, GAME
};

class TeapotEngine {
	
public:
	GameState gameState;

	
	TeapotEngine();
	~TeapotEngine();
	
	
};

#endif