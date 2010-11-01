#ifndef TEXTURE_H
#define TEXTURE_H

/**
Texture class for holding any textures
**/

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

#include "MyBitmap.h"

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