#include "Texture.h"


Texture::Texture(void)
{
	this->textureMap = NULL;
}

Texture::Texture(const char* filename)
{
	this->loadTexture(filename);
}

Texture::~Texture(void)
{
	delete this->textureMap;
	delete this;
}

void Texture::loadTexture(const char* filename)
{
	this->textureMap = new MyBitmap(filename);
	if (!this->textureMap->loaded){
		cout << "Could not load texture. Error: " << this->textureMap->error << endl;
		exit(0);
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &this->textureID);				//Allocate Texture Name
	glBindTexture(GL_TEXTURE_2D, this->textureID);	//Select current texture

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->textureMap->width, this->textureMap->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->textureMap->data);
	
}
