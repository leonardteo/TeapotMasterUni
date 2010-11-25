/*
 *  Matrix4.cpp
 *  TeapotMaster
 *
 *  Created by Leonard Teo on 10-11-25.
 *  Copyright 2010 Leonard Teo. All rights reserved.
 *
 */

#include "Matrix4.h"

Matrix4::Matrix4()
{
	for (int i=0; i<16; i++)
		this->m[i] = 0.0f;	
}

Matrix4::Matrix4(float in[16])
{
	for (int i=0; i<16; i++)
		this->m[i] = in[i];
}

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


void Matrix4::print()
{
	for (int i=0; i<4; i++)
	{
		cout << this->m[0+i] << " ";
		cout << this->m[4+i] << " ";
		cout << this->m[8+i] << " ";
		cout << this->m[12+i] << " ";
		cout << endl;		
	}	
}

Matrix4::~Matrix4()
{
	
}
