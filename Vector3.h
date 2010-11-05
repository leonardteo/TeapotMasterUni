
/**
Vector3 class
Simple storage for Vector3 type
Will expand later when I figure out something useful to do with this.
@author Leonard Teo
**/

#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>
#include <math.h>

class Vector3
{
public:

	float x, y, z;

	Vector3(void);
	Vector3(float nx, float ny, float nz);
	~Vector3(void);

	//Do a cross product of a vector and return it
	Vector3* crossProduct(Vector3* in);
	
	//Normalize the current vector
	void normalize();
	
	//length/distance
	float length();

	//Overload the << operator for easily debugging values
	friend std::ostream& operator<<(std::ostream& output, const Vector3* v);

};

#endif

