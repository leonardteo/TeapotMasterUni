#include "CollisionPlane.h"


CollisionPlane::CollisionPlane(Vector3 point1, Vector3 point2, Vector3 point3)
{
	//Copy the points
	this->v1 = point1;
	this->v2 = point2;
	this->v3 = point3;

	//Generate the normal
	Vector3 a = this->v2 - this->v1;
	Vector3 b = this->v3 - this->v1;
	this->normal = a.crossProduct(b);
	this->normal.normalize();
	this->d = -(this->normal.x * this->v1.x + this->normal.y * this->v1.y + this->normal.z * this->v1.z);

}

float CollisionPlane::classifyPoint(Vector3 point)
{
	//Solve for plane
	return (dotProduct(this->normal, point) + this->d);
}

bool CollisionPlane::insideTriangle(Vector3 point, float sphereRadius)
{
	float distance = this->classifyPoint(point);

	//If point is on the wrong side, return false because we don't care
	if (distance < sphereRadius)
	{
		return false;
	}

	//Project point onto triangle then check if it is inside the triangle
	Vector3 direction = this->normal;
	direction.reverse();

	//The point to test
	point = point + (direction * distance);

	//Test using cross products - all cross products must be in the same direction
	Vector3 vec1 = this->v2 - this->v1;
	Vector3 vec2 = this->v3 - this->v2;
	Vector3 vec3 = this->v1 - this->v3;

	Vector3 v1p = point - this->v1;
	Vector3 v2p = point - this->v2;
	Vector3 v3p = point - this->v3;

	Vector3 n1 = vec1.crossProduct(v1p);
	Vector3 n2 = vec2.crossProduct(v2p);
	Vector3 n3 = vec3.crossProduct(v3p);

	/*
	cout << "n1: " << n1 << endl;
	cout << "n2: " << n2 << endl;
	cout << "n3: " << n3 << endl << endl;
	*/

	float d1 = dotProduct(n1, this->normal);
	float d2 = dotProduct(n2, this->normal);
	float d3 = dotProduct(n3, this->normal);

	/*
	cout << "d1: " << d1 << endl;
	cout << "d2: " << d2 << endl;
	cout << "d3: " << d3 << endl << endl;
	*/

	if (d1 > 0.0f && d2 > 0.0f && d3 > 0.0f)
	{
		return true;
	} 

	return false;

	
}

CollisionPlane::~CollisionPlane(void)
{
}

