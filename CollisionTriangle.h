#ifndef COLLISION_TRIANGLE_H
#define COLLISION_TRIANGLE_H

#include <math.h>
#include "Vector3.h"

class CollisionTriangle
{
public:

	//Data Members
	Vector3 v1;
	Vector3 v2;
	Vector3 v3;
	Vector3 normal;
	float d;

	//Constructor
	CollisionTriangle(Vector3 point1, Vector3 point2, Vector3 point3);
	~CollisionTriangle(void);

	//Checks if a point is on the plane or not
	float distanceToPoint(Vector3 point);
	float sphereCollision(Vector3 center, float radius);

	//Checks if a point is inside the triangle defining the plane
	//Optional sphereRadius to check that the sphere really doesn't intersect
	bool insideTriangle(Vector3 point);

	//Edge/sphere intersection. For nasty corners
	bool sphereEdgeCollision(Vector3 sphereCenter, float sphereRadius);

private:
	
	bool segmentSphereIntersection(Vector3 segmentOrigin, Vector3 segmentDirection, float segmentExtent, Vector3 sphereCenter, float sphereRadius);
	bool raySphereIntersection(Vector3 rayOrigin, Vector3 rayDirection, Vector3 sphereCenter, float sphereRadius);


};

#endif