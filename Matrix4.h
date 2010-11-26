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
	
};

#endif