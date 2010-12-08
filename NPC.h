#ifndef NPC_H
#define NPC_H

#include "TransformNode.h"
#include "PolyMeshNode.h"
#include "Matrix4.h"
#include "Vector3.h"

/**
Non-player character class
**/

class NPC : public TransformNode
{
public:
	NPC(string id, PolyMeshNode* mesh);
	~NPC(void);

	//Data members
	PolyMeshNode* meshNode;
	PolyMeshNode* exclamation;
	Vector3* heading;
	float velocity;
	bool alarmed;
	bool finished;

	//Collision Detection
	void setBoundingSphereRadius(float radius);
	float boundingSphereRadius;

	//Methods
	void rotateHeading(float angle);
};

#endif