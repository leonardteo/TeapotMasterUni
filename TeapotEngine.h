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
#include <SDL/SDL.h>
#include <GLEW/GLEW.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/GLUT.h>
#else
#include <Windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>	
#include <SDL.h>
#endif

#include <stdlib.h>

#include "TeapotLevel.h"


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