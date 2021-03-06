#ifndef TEXTURE_H
#define TEXTURE_H

/**
Texture class for holding any textures
**/

//OpenGL libraries
#ifdef __APPLE__
#include <GLEW/GLEW.h>
#include <GLUT/GLUT.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "MyBitmap.h"
#include <SOIL.h>

class Texture
{
public:

	//Constructor/Destructor
	Texture(void);
	Texture(const char* filename);
	~Texture(void);

	//Bitmap
	MyBitmap* textureMap;
	GLuint textureID;

private:
	void loadTexture(const char* filename);

};


#endif