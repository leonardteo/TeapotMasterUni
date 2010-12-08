/*
 *  Matrix4.h
 *  TeapotMaster
 *
 *  Created by Leonard Teo on 10-11-25.
 *  Copyright 2010 Leonard Teo. All rights reserved.
 *
 */

#ifndef MATRIX4_H
#define MATRIX4_H

#include <iostream>
#include <math.h>
#include "Vector3.h"

using namespace std;

class Matrix4
{
public:
	float m[16];
	
	//Methods
	Matrix4();
	Matrix4(float in[16]);
	~Matrix4();
	
	void loadIdentity();
	float determinant();
	Matrix4 inverse();
	void print();
	void rotate(Vector3 axis, float angle);

	//Overloaded math functions
	Vector3 operator * (Vector3 in);	
	Matrix4 operator * (Matrix4 in);

private:
	Matrix4 multiply(Matrix4 in);
	Vector3 multiply(Vector3 in);
	
};

/**
Non-member functions
**/

//Create a rotation matrix
//Formula for rotation on arbitrary axis is here:
//http://en.wikipedia.org/wiki/Rotation_matrix
static Matrix4 rotationMatrix(Vector3 axis, float angle)
{
	const float degreesToRadians = 0.0174532925f;

	angle = angle * degreesToRadians;

	float out[16];

	out[0] = cos(angle) + axis.x * axis.x * (1 - cos(angle));
	out[1] = axis.y * axis.x  * (1 - cos(angle)) + axis.z * sin(angle);
	out[2] = axis.z * axis.z * (1 - cos(angle)) - axis.y * sin(angle);
	out[3] = 0.0f;

	out[4] = axis.x * axis.y * (1 - cos(angle)) - axis.z * sin(angle);
	out[5] = cos(angle) + axis.y * axis.y * (1 - cos(angle));
	out[6] = axis.z * axis.y * (1 - cos(angle)) + axis.x * sin(angle);
	out[7] = 0.0f;

	out[8] = axis.x * axis.z * (1 - cos(angle)) + axis.y * sin(angle);
	out[9] = axis.y * axis.z * (1 - cos(angle)) - axis.x * sin(angle);
	out[10] = cos(angle) + axis.z * axis.z * (1 - cos(angle));
	out[11] = 0.0f;

	out[12] = 0.0f;
	out[13] = 0.0f;
	out[14] = 0.0f;
	out[15] = 1.0f;

	Matrix4 outm = Matrix4(out);
	return outm;
}

#endif