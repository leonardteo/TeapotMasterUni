#ifndef COLLISION_PLANE_H
#define COLLISION_PLANE_H

#include <math.h>
#include "Vector3.h"

class CollisionPlane
{
public:

	//Data Members
	Vector3 v1;
	Vector3 v2;
	Vector3 v3;
	Vector3 normal;
	float d;

	//Constructor
	CollisionPlane(Vector3 point1, Vector3 point2, Vector3 point3);
	~CollisionPlane(void);

	//Checks if a point is on the plane or not
	float classifyPoint(Vector3 point);

	//Checks if a point is inside the triangle defining the plane
	//Optional sphereRadius to check that the sphere really doesn't intersect
	bool insideTriangle(Vector3 point, float sphereRadius = 0.0f);

};

#endif