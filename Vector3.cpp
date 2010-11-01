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

//Overloaded << operator
std::ostream& operator<<(std::ostream& output, const Vector3* v) {
	output << "(" <<  v->x << ", " << v->y <<", " << v->z << ")";
    return output;  // for multiple << operators.
}
