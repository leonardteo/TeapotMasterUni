/*
 *  Matrix4.cpp
 *  TeapotMaster
 *
 *  Created by Leonard Teo on 10-11-25.
 *  Copyright 2010 Leonard Teo. All rights reserved.
 *
 */

#include "Matrix4.h"

/*/
Constructor
**/
Matrix4::Matrix4()
{
	this->loadIdentity();
}

/**
Initialize an identity matrix
**/
void Matrix4::loadIdentity()
{
	this->m[0] = 1.0f;	this->m[4] = 0.0f;	this->m[8] = 0.0f;	this->m[12] = 0.0f;
	this->m[1] = 0.0f;	this->m[5] = 1.0f;	this->m[9] = 0.0f;	this->m[13] = 0.0f;
	this->m[2] = 0.0f;	this->m[6] = 0.0f;	this->m[10] = 1.0f;	this->m[14] = 0.0f;
	this->m[3] = 0.0f;	this->m[7] = 0.0f;	this->m[11] = 0.0f;	this->m[15] = 1.0f;
}

/**
Copy in an array of floats
**/
Matrix4::Matrix4(float in[16])
{
	for (int i=0; i<16; i++)
		this->m[i] = in[i];
}

/**
Calculate the determinant of the matrix
**/
float Matrix4::determinant()
{
	return
	this->m[12] * this->m[9] * this->m[6] * this->m[3] -
	this->m[8] * this->m[13] * this->m[6] * this->m[3]-
	this->m[12] * this->m[5] * this->m[10] * this->m[3]+
	this->m[4] * this->m[13] * this->m[10] * this->m[3]+
	this->m[8] * this->m[5] * this->m[14] * this->m[3]-
	this->m[4] * this->m[9] * this->m[14] * this->m[3]-
	this->m[12] * this->m[9] * this->m[2] * this->m[7]+
	this->m[8] * this->m[13] * this->m[2] * this->m[7]+
	this->m[12] * this->m[1] * this->m[10] * this->m[7]-
	this->m[0] * this->m[13] * this->m[10] * this->m[7]-
	this->m[8] * this->m[1] * this->m[14] * this->m[7]+
	this->m[0] * this->m[9] * this->m[14] * this->m[7]+
	this->m[12] * this->m[5] * this->m[2] * this->m[11]-
	this->m[4] * this->m[13] * this->m[2] * this->m[11]-
	this->m[12] * this->m[1] * this->m[6] * this->m[11]+
	this->m[0] * this->m[13] * this->m[6] * this->m[11]+
	this->m[4] * this->m[1] * this->m[14] * this->m[11]-
	this->m[0] * this->m[5] * this->m[14] * this->m[11]-
	this->m[8] * this->m[5] * this->m[2] * this->m[15]+
	this->m[4] * this->m[9] * this->m[2] * this->m[15]+
	this->m[8] * this->m[1] * this->m[6] * this->m[15]-
	this->m[0] * this->m[9] * this->m[6] * this->m[15]-
	this->m[4] * this->m[1] * this->m[10] * this->m[15]+
	this->m[0] * this->m[5] * this->m[10] * this->m[15];	
}

/**
Inverse the matrix
**/
Matrix4 Matrix4::inverse()
{
	float x = this->determinant();
	if (x == 0) return false;
	
	float i[16];
	
	i[0]= (-this->m[13]*this->m[10]*this->m[7] +this->m[9]*this->m[14]*this->m[7] +this->m[13]*this->m[6]*this->m[11]
		   -this->m[5]*this->m[14]*this->m[11] -this->m[9]*this->m[6]*this->m[15] +this->m[5]*this->m[10]*this->m[15])/x;
	i[4]= ( this->m[12]*this->m[10]*this->m[7] -this->m[8]*this->m[14]*this->m[7] -this->m[12]*this->m[6]*this->m[11]
		   +this->m[4]*this->m[14]*this->m[11] +this->m[8]*this->m[6]*this->m[15] -this->m[4]*this->m[10]*this->m[15])/x;
	i[8]= (-this->m[12]*this->m[9]* this->m[7] +this->m[8]*this->m[13]*this->m[7] +this->m[12]*this->m[5]*this->m[11]
		   -this->m[4]*this->m[13]*this->m[11] -this->m[8]*this->m[5]*this->m[15] +this->m[4]*this->m[9]* this->m[15])/x;
	i[12]=( this->m[12]*this->m[9]* this->m[6] -this->m[8]*this->m[13]*this->m[6] -this->m[12]*this->m[5]*this->m[10]
		   +this->m[4]*this->m[13]*this->m[10] +this->m[8]*this->m[5]*this->m[14] -this->m[4]*this->m[9]* this->m[14])/x;
	i[1]= ( this->m[13]*this->m[10]*this->m[3] -this->m[9]*this->m[14]*this->m[3] -this->m[13]*this->m[2]*this->m[11]
		   +this->m[1]*this->m[14]*this->m[11] +this->m[9]*this->m[2]*this->m[15] -this->m[1]*this->m[10]*this->m[15])/x;
	i[5]= (-this->m[12]*this->m[10]*this->m[3] +this->m[8]*this->m[14]*this->m[3] +this->m[12]*this->m[2]*this->m[11]
		   -this->m[0]*this->m[14]*this->m[11] -this->m[8]*this->m[2]*this->m[15] +this->m[0]*this->m[10]*this->m[15])/x;
	i[9]= ( this->m[12]*this->m[9]* this->m[3] -this->m[8]*this->m[13]*this->m[3] -this->m[12]*this->m[1]*this->m[11]
		   +this->m[0]*this->m[13]*this->m[11] +this->m[8]*this->m[1]*this->m[15] -this->m[0]*this->m[9]* this->m[15])/x;
	i[13]=(-this->m[12]*this->m[9]* this->m[2] +this->m[8]*this->m[13]*this->m[2] +this->m[12]*this->m[1]*this->m[10]
		   -this->m[0]*this->m[13]*this->m[10] -this->m[8]*this->m[1]*this->m[14] +this->m[0]*this->m[9]* this->m[14])/x;
	i[2]= (-this->m[13]*this->m[6]* this->m[3] +this->m[5]*this->m[14]*this->m[3] +this->m[13]*this->m[2]*this->m[7]
		   -this->m[1]*this->m[14]*this->m[7] -this->m[5]*this->m[2]*this->m[15] +this->m[1]*this->m[6]* this->m[15])/x;
	i[6]= ( this->m[12]*this->m[6]* this->m[3] -this->m[4]*this->m[14]*this->m[3] -this->m[12]*this->m[2]*this->m[7]
		   +this->m[0]*this->m[14]*this->m[7] +this->m[4]*this->m[2]*this->m[15] -this->m[0]*this->m[6]* this->m[15])/x;
	i[10]=(-this->m[12]*this->m[5]* this->m[3] +this->m[4]*this->m[13]*this->m[3] +this->m[12]*this->m[1]*this->m[7]
		   -this->m[0]*this->m[13]*this->m[7] -this->m[4]*this->m[1]*this->m[15] +this->m[0]*this->m[5]* this->m[15])/x;
	i[14]=( this->m[12]*this->m[5]* this->m[2] -this->m[4]*this->m[13]*this->m[2] -this->m[12]*this->m[1]*this->m[6]
		   +this->m[0]*this->m[13]*this->m[6] +this->m[4]*this->m[1]*this->m[14] -this->m[0]*this->m[5]* this->m[14])/x;
	i[3]= ( this->m[9]* this->m[6]* this->m[3] -this->m[5]*this->m[10]*this->m[3] -this->m[9]* this->m[2]*this->m[7]
		   +this->m[1]*this->m[10]*this->m[7] +this->m[5]*this->m[2]*this->m[11] -this->m[1]*this->m[6]* this->m[11])/x;
	i[7]= (-this->m[8]* this->m[6]* this->m[3] +this->m[4]*this->m[10]*this->m[3] +this->m[8]* this->m[2]*this->m[7]
		   -this->m[0]*this->m[10]*this->m[7] -this->m[4]*this->m[2]*this->m[11] +this->m[0]*this->m[6]* this->m[11])/x;
	i[11]=( this->m[8]* this->m[5]* this->m[3] -this->m[4]*this->m[9]* this->m[3] -this->m[8]* this->m[1]*this->m[7]
		   +this->m[0]*this->m[9]* this->m[7] +this->m[4]*this->m[1]*this->m[11] -this->m[0]*this->m[5]* this->m[11])/x;
	i[15]=(-this->m[8]* this->m[5]* this->m[2] +this->m[4]*this->m[9]* this->m[2] +this->m[8]* this->m[1]*this->m[6]
		   -this->m[0]*this->m[9]* this->m[6] -this->m[4]*this->m[1]*this->m[10] +this->m[0]*this->m[5]* this->m[10])/x;
	
	Matrix4 returnMatrix(i);
	return returnMatrix;
}

/**
Multiply the current matrix by an incoming vector
**/
Vector3 Matrix4::multiply(Vector3 in)
{
	float x = this->m[0] * in.x + this->m[4] * in.y + this->m[8] * in.z + this->m[12];
	float y = this->m[1] * in.x + this->m[5] * in.y + this->m[9] * in.z + this->m[13];
	float z = this->m[2] * in.x + this->m[6] * in.y + this->m[10] * in.z + this->m[14];

	Vector3 out(x, y, z);
	return out;
}

/**
Multiply the current matrix by another matrix. Sets current matrix to the resulting matrix
**/
Matrix4 Matrix4::multiply(Matrix4 in)
{
	float out[16];

	out[0] = this->m[0] * in.m[0]  +  this->m[4] * in.m[1]  +  this->m[8] * in.m[2]  +  this->m[12] * in.m[3];
	out[1] = this->m[1] * in.m[0]  +  this->m[5] * in.m[1]  +  this->m[9] * in.m[2]  +  this->m[13] * in.m[3];
	out[2] = this->m[2] * in.m[0]  +  this->m[6] * in.m[1]  +  this->m[10] * in.m[2]  +  this->m[14] * in.m[3];
	out[3] = this->m[3] * in.m[0]  +  this->m[7] * in.m[1]  +  this->m[11] * in.m[2]  +  this->m[15] * in.m[3];

	out[4] = this->m[0] * in.m[4]  +  this->m[4] * in.m[5]  +  this->m[8] * in.m[6]  +  this->m[12] * in.m[7];
	out[5] = this->m[1] * in.m[4]  +  this->m[5] * in.m[5]  +  this->m[9] * in.m[6]  +  this->m[13] * in.m[7];
	out[6] = this->m[2] * in.m[4]  +  this->m[6] * in.m[5]  +  this->m[10] * in.m[6]  +  this->m[14] * in.m[7];
	out[7] = this->m[3] * in.m[4]  +  this->m[7] * in.m[5]  +  this->m[11] * in.m[6]  +  this->m[15] * in.m[7];

	out[8] = this->m[0] * in.m[8]  +  this->m[4] * in.m[9]  +  this->m[8] * in.m[10]  +  this->m[12] * in.m[11];
	out[9] = this->m[1] * in.m[8]  +  this->m[5] * in.m[9]  +  this->m[9] * in.m[10]  +  this->m[13] * in.m[11];
	out[10] = this->m[2] * in.m[8]  +  this->m[6] * in.m[9]  +  this->m[10] * in.m[10]  +  this->m[14] * in.m[11];
	out[11] = this->m[3] * in.m[8]  +  this->m[7] * in.m[9]  +  this->m[11] * in.m[10]  +  this->m[15] * in.m[11];

	out[12] = this->m[0] * in.m[12]  +  this->m[4] * in.m[13]  +  this->m[8] * in.m[14]  +  this->m[12] * in.m[15];
	out[13] = this->m[1] * in.m[12]  +  this->m[5] * in.m[13]  +  this->m[9] * in.m[14]  +  this->m[13] * in.m[15];
	out[14] = this->m[2] * in.m[12]  +  this->m[6] * in.m[13]  +  this->m[10] * in.m[14]  +  this->m[14] * in.m[15];
	out[15] = this->m[3] * in.m[12]  +  this->m[7] * in.m[13]  +  this->m[11] * in.m[14]  +  this->m[15] * in.m[15];

	Matrix4 outM = Matrix4(out);
	return outM;
	
}

/**
Overloaded Operators
**/
//Overloaded * operator
Vector3 Matrix4::operator* (Vector3 in)
{
	return this->multiply(in);
}

Matrix4 Matrix4::operator* (Matrix4 in)
{
	return this->multiply(in);
}

void Matrix4::rotate(Vector3 axis, float angle)
{
	Matrix4 rotMatrix = rotationMatrix(axis, angle);
	Matrix4 outM = this->multiply(rotMatrix);

	for (int i=0; i<16; i++)
	{
		this->m[i] = outM.m[i];
	}
}

/**
Debugging tool to print out the matrix
**/
void Matrix4::print()
{

	for (int i=0; i<4; i++)
	{
		cout << setprecision(3) << fixed << this->m[0+i] << " ";
		cout << setprecision(3) << fixed << this->m[4+i] << " ";
		cout << setprecision(3) << fixed << this->m[8+i] << " ";
		cout << setprecision(3) << fixed << this->m[12+i] << " ";
		cout << setprecision(3) << fixed << endl;		
	}	
}

Matrix4::~Matrix4()
{
	
}

