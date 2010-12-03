/*
 *  Grid.h
 *  TeapotMaster
 *
 *  Created by Leonard Teo on 10-11-04.
 *  Copyright 2010 Leonard Teo. All rights reserved.
 *
 */

#ifndef GRID_H
#define GRID_H

//OpenGL libraries
#ifdef __APPLE__
#include <GLEW/GLEW.h>
#include <GLUT/GLUT.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

class Grid{
	
public:
	//Grid();
	//~Grid();
	void display(float scale = 1, int n=48);
};

#endif