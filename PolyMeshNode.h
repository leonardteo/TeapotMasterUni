/*
 *  PolyMesh.h
 *  OBJViewer
 *
 *  Created by Leonard Teo on 10-10-05.
 *
 */

#ifndef POLYMESHNODE_H
#define POLYMESHNODE_H

#include <list>

#include "Node.h"
#include "OBJModel.h"
#include "Texture.h"
#include "Material.h"
#include "CollisionTriangle.h"

//OpenGL libraries
#ifdef __APPLE__
#include <GLEW/GLEW.h>
#include <GLUT/GLUT.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

using namespace std;

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))

class PolyMeshNode : public Node {

public:

	//Data members
	OBJModel* mesh;
	Texture* texture;
	Material* material;
	CollisionTriangle** collisionTriangles;
	int numFaces;

	//Collision Detection - if using polymesh as a collider object
	bool staticCollider;
	bool activeCollider;
	float colliderSphereRadius;

	//Constructor/Destructor
	PolyMeshNode(string id = "");
	PolyMeshNode(string id, string filename);
	~PolyMeshNode();

	//Attach model - note that this class does NOT load OBJModels. It attaches them so that you can have instances of models
	void attachModel(OBJModel* model);

	//Attach texture
	void attachTexture(Texture* texture);
	void attachTexture(string filename);

	void attachMaterial(Material* material);

	//Draw the polygons using OpenGL calls
	void draw();
	
	//Scene graph rendering call
	void render(RenderType renderType = ALL_OBJECTS);

	//Collision Detection methods
	void initStaticCollider();

private:
	void init();

};

#endif
