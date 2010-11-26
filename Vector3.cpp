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

Vector3 Vector3::crossProduct(Vector3 in)
{
	Vector3 out;
	
	out.x = (this->y * in.z) - (this->z * in.y);
	out.y = (this->z * in.x) - (this->x * in.z);
	out.z = (this->x * in.y) - (this->y * in.x);
	
	return out;
}

float Vector3::length()
{
	return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
}

void Vector3::normalize()
{
	float length = this->length();
	
	if (length > 0.0f || length < 0.0f){
		
		this->x = this->x/length;
		this->y = this->y/length;
		this->z = this->z/length;
	}
}



//Overloaded << operator
std::ostream& operator<<(std::ostream& output, const Vector3* v) {
	output << setprecision(3) << fixed << "(" <<  v->x << ", " << v->y <<", " << v->z << ")";
    return output;  // for multiple << operators.
}


//Overloaded << operator
std::ostream& operator<<(std::ostream& output, const Vector3 v) {
	output << setprecision(3) << fixed << "(" <<  v.x << ", " << v.y <<", " << v.z << ")";
    return output;  // for multiple << operators.
}

//Overloaded + operator
Vector3 Vector3::operator+ (Vector3 in)
{
	return Vector3(this->x + in.x, this->y + in.y, this->z + in.z);
}

//Overloaded - operator
Vector3 Vector3::operator- (Vector3 in)
{
	return Vector3(this->x - in.x, this->y - in.y, this->z - in.z);
}

//Overloaded * operator
Vector3 Vector3::operator* (float scalar)
{
	return Vector3(this->x * scalar, this->y * scalar, this->z * scalar);
}

//Overloaded / operator
Vector3 Vector3::operator/ (float scalar)
{
	return Vector3(this->x / scalar, this->y / scalar, this->z / scalar);
}

void Vector3::zero()
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
}