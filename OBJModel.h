/**
My attempt at an OBJ parser
Leonard Teo

Done:
Only supports triangles - remember to triangulate all models

To DO:
- Generate normals if a model doesn't have them
- Handle groups and materials

**/

#ifndef OBJMODEL_H
#define OBJMODEL_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>

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

//Teapot Engine Libraries
#include "Vector3.h"

using namespace std;

struct UV 
{
	float u;
	float v;
};

class OBJModel
{

friend class PolyMeshNode;	//Make sure that PolyMeshNode can access all data in this class

public:
	//Constructor
	OBJModel(const char* fileName);

	//Destructor
	~OBJModel(void);

	//Generate normals - TO DO
	void generateNormals();

private:

	//Private data members
	//Total number of vertices
	int numVertices;
	int numFaces;
	int numUVs;
	int numNormals;
	
	//Data
	Vector3** vertices;
	Vector3** normals;
	UV** uvs;
	int** index;			//2 dimensional arrays: Note these are pointers so that we can do this: this->normals_index[i][j]
	int** normalsIndex;
	int** uvIndex;

	GLfloat* vertexArray;	
	GLfloat* normalsArray;
	GLfloat* uvArray;

	GLuint* indexArray; //Straight line array

	//Vertex buffer object IDs
	GLuint vbo_vertices;
	GLuint vbo_normals;
	GLuint vbo_uvs;
	GLuint vbo_index;

	//Initialize the arrays and variables
	void init();

	//Load a model file
	void load(const char* fileName);

	//Prepare arrays
	void prepArrays();
	void debugArrays();

	
};

#endif