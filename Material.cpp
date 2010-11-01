#include "Material.h"


Material::Material(void)
{
	//Default material
	this->ambient[0] = 0.0f;
	this->ambient[1] = 0.0f;
	this->ambient[2] = 0.0f;
	this->ambient[3] = 1.0f;

	this->diffuse[0] = 0.9f;
	this->diffuse[1] = 0.9f;
	this->diffuse[2] = 0.9f;
	this->diffuse[3] = 1.0f;

	this->specular[0] = 0.8f;
	this->specular[1] = 0.8f;
	this->specular[2] = 0.8f;
	this->specular[3] = 1.0f;

	this->shininess[0] = 5.0f;

}

void Material::setAmbient(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	this->ambient[0] = red;
	this->ambient[1] = green;
	this->ambient[2] = blue;
	this->ambient[3] = alpha;
}
void Material::setDiffuse(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	this->diffuse[0] = red;
	this->diffuse[1] = green;
	this->diffuse[2] = blue;
	this->diffuse[3] = alpha;
}
void Material::setDiffuseAndAmbient(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	this->setDiffuse(red, green, blue, alpha);
	this->setAmbient(red, green, blue, alpha);
}
void Material::setSpecular(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	this->specular[0] = red;
	this->specular[1] = green;
	this->specular[2] = blue;
	this->specular[3] = alpha;
}
void Material::setShininess(GLfloat power)
{
	this->shininess[0] = power;
}

Material::~Material(void)
{
}
