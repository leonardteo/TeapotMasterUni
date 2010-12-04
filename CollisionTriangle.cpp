#include "CollisionTriangle.h"


CollisionTriangle::CollisionTriangle(Vector3 point1, Vector3 point2, Vector3 point3)
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

float CollisionTriangle::distanceToPoint(Vector3 point)
{
	//Solve for plane
	return (dotProduct(this->normal, point) + this->d);
}

float CollisionTriangle::sphereCollision(Vector3 center, float radius)
{
	float distance = this->distanceToPoint(center);
	if (distance > radius)
	{
		return distance - radius;
	} else if (distance < -radius)
	{
		return distance + radius;
	} else {
		return 0.0f;
	}
}



bool CollisionTriangle::insideTriangle(Vector3 point, float sphereRadius)
{
	float distance = this->distanceToPoint(point);

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

bool CollisionTriangle::sphereEdgeCollision(Vector3 sphereCenter, float sphereRadius)
{
	//Test intersection for each edge
	Vector3 ray1 = this->v2 - this->v1;	
	Vector3 ray2 = this->v3 - this->v2;
	Vector3 ray3 = this->v1 - this->v3;

	//Calculate segments
	float s1Extent = ray1.length();
	float s2Extent = ray2.length();
	float s3Extent = ray3.length();
	ray1.normalize();
	ray2.normalize();
	ray3.normalize();

	//Do the segment intersection test
	bool s1Test = this->segmentSphereIntersection(this->v1, ray1, s1Extent, sphereCenter, sphereRadius);
	bool s2Test = this->segmentSphereIntersection(this->v2, ray2, s2Extent, sphereCenter, sphereRadius);
	bool s3Test = this->segmentSphereIntersection(this->v3, ray3, s3Extent, sphereCenter, sphereRadius);

	return (s1Test || s2Test|| s3Test);

	/* Ray intersection code - rays are infinite, so this doesn't work
	bool ray1Int = this->raySphereIntersection(this->v1, ray1, sphereCenter, sphereRadius);
	bool ray2Int = this->raySphereIntersection(this->v2, ray2, sphereCenter, sphereRadius);
	bool ray3Int = this->raySphereIntersection(this->v3, ray3, sphereCenter, sphereRadius);

	return (ray1Int || ray2Int || ray3Int);
	*/
}

//Ray sphere intersection
bool CollisionTriangle::raySphereIntersection(Vector3 rayOrigin, Vector3 rayDirection, Vector3 sphereCenter, float sphereRadius)
{
	Vector3 w = sphereCenter - rayOrigin;
	float wsq = dotProduct(w, w);
	float proj = dotProduct(w, rayDirection);
	float rsq = sphereRadius * sphereRadius;

	//If sphere is behind ray, no intersection
	if (proj < 0.0f && wsq > rsq) 
		return false;

	float vsq = dotProduct(rayDirection, rayDirection);

	return (vsq * wsq - proj * proj <= vsq * sphereRadius * sphereRadius);
}

//Segment/Sphere intersection
bool CollisionTriangle::segmentSphereIntersection(Vector3 segmentOrigin, Vector3 segmentDirection, float segmentExtent, Vector3 sphereCenter, float sphereRadius)
{
	Vector3 delta = segmentOrigin - sphereCenter;

	//Trivial test - check if P is inside of on the sphere
	float a0 = dotProduct(delta, delta) - sphereRadius * sphereRadius;
	if (a0 <= 0)
	{
		return true;
	}

	float a1 = dotProduct(segmentDirection, delta);
	float discr = a1 * a1 - a0;
	if (discr < 0)
	{
		//Two complex valued roots therefore no intersection
		return false;
	}

	float absA1 = fabs(a1);
	float qval = segmentExtent * (segmentExtent - 2 * absA1) + a0;
	return qval <= 0 || absA1 <= segmentExtent;
	
}

CollisionTriangle::~CollisionTriangle(void)
{
}

