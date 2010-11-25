
/**
Vector3 class
Simple storage for Vector3 type
Will expand later when I figure out something useful to do with this.
@author Leonard Teo
**/

#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

class Vector3
{
public:

	float x, y, z;

	Vector3(void);
	Vector3(float nx, float ny, float nz);
	~Vector3(void);

	//Do a cross product of a vector and return it
	Vector3 crossProduct(Vector3 in);
	
	//Normalize the current vector
	void normalize();
	
	//length/distance
	float length();

	//Overload the << operator for easily debugging values
	friend std::ostream& operator<<(std::ostream& output, const Vector3* v);
	friend std::ostream& operator<<(std::ostream& output, const Vector3 v);
	
	//Math operator overloads
	Vector3 operator + (Vector3);
	Vector3 operator - (Vector3);
	Vector3 operator * (float);	//Scalar multiply
	Vector3 operator / (float); //Scalar divide

};

/**
 Non-member functions
 **/
static float dotProduct(Vector3* a, Vector3* b)
{
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

static float dotProduct(Vector3 a, Vector3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

static Vector3 crossProduct(Vector3 a, Vector3 b)
{
	Vector3 out;
	out.x = a.y * b.z - a.z * b.y;
	out.y = a.z * b.x - a.x * b.z;
	out.z = a.x * b.y - a.y * b.x;
	return out;
}

#endif

