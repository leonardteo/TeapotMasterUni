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
	NPC(string id, Node* node);
	~NPC(void);

	//Data members

	//Scene graph structure
	//meshNode->exclamation
	//meshNode->exclamation
	//meshNode->leftFoot;
	//meshNode->rightFoot;
	//this->wobbleNode->meshNode
	Node* meshNode;
	PolyMeshNode* exclamation;
	PolyMeshNode* leftFoot;
	PolyMeshNode* rightFoot;
	TransformNode* wobbleNode;

	float animationT;	//Artificial coefficient for helping with animation

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