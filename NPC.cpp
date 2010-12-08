#include "NPC.h"


NPC::NPC(string id, Node* node) : TransformNode(id, TRANSLATE)
{
	this->meshNode = node;
	this->exclamation = NULL;
	this->leftFoot = NULL;
	this->rightFoot = NULL;
	this->wobbleNode = NULL;
	this->addChild(this->meshNode);
	
	//Initialize speed and heading
	this->velocity = 0.2f;
	this->heading = new Vector3(0.0f, 0.0f, 1.0f);

	//Initialize collision detection
	this->boundingSphereRadius = 0.0f;

	this->alarmed = false;
	this->finished = false;

	this->animationT = 0.0f;
}

void NPC::setBoundingSphereRadius(float radius)
{
	this->boundingSphereRadius = radius;
	this->meshNode->setTranslation(0.0f, -radius, 0.0f);	
}

//Change the heading by the angle and rotate the model
void NPC::rotateHeading(float angle)
{
	//Rotate the model
	//this->rotationNode->rotateBall(axis, angle);
	this->meshNode->rotate->y += angle;

	Vector3 axis(0.0f, 1.0f, 0.0f);

	//Rotate the heading vector
	Matrix4 rotMatrix = rotationMatrix(axis, angle);
	Vector3 newHeading = rotMatrix * *this->heading;
	
	this->heading = new Vector3(newHeading.x, newHeading.y, newHeading.z);
	
}

NPC::~NPC(void)
{
}
