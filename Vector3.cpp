#include "Vector3.h"

Vector3::Vector3(void)
{
	x = 0;
	y = 0;
	z = 0;
}

Vector3::Vector3(float nx, float ny, float nz)
{
	x = nx;
	y = ny;
	z = nz;
}

Vector3::~Vector3(void)
{
}

Vector3* Vector3::crossProduct(Vector3* in)
{
	Vector3* out = new Vector3();
	
	out->x = (this->y * in->z) - (this->z * in->y);
	out->y = (this->z * in->x) - (this->x * in->z);
	out->z = (this->x * in->y) - (this->y * in->x);
	
	return out;
}

float Vector3::length()
{
	return sqrt((this->x * this->x) + (this->y * this->y) + (this->z + this->z));
}

void Vector3::normalize()
{
	if (this->length() > 0.0f || this->length() < 0.0f){
		this->x = this->x/length();
		this->y = this->y/length();
		this->z = this->z/length();
	}
}

//Overloaded << operator
std::ostream& operator<<(std::ostream& output, const Vector3* v) {
	output << "(" <<  v->x << ", " << v->y <<", " << v->z << ")";
    return output;  // for multiple << operators.
}
