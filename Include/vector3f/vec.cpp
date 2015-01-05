#include <vector3f/vec.h>

//--------------------------------------------------
vec3f::vec3f(vec3f const& v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
}

//--------------------------------------------------
vec3f vec3f::operator*(float x)
{
    return vec3f(this->x * x, this->y * x, this->z * x);
}

//--------------------------------------------------
vec3f vec3f::operator+(vec3f const& op2)
{
	return vec3f(this->x + op2.x, this->y + op2.y, this->z + op2.z);
}

//--------------------------------------------------
vec3f vec3f::operator-(vec3f const& op2)
{
	return vec3f(this->x - op2.x, this->y - op2.y, this->z - op2.z);
}

//--------------------------------------------------
vec3f vec3f::operator=(vec3f const& op2)
{
	this->x = op2.x;
	this->y = op2.y;
	this->z = op2.z;

	return *this;
}

//--------------------------------------------------
vec3f vec3f::operator*(vec3f const& op2)
{
	return vec3f(this->x * op2.x, this->y * op2.y, this->z * op2.z);
}

//-------------------------------------------------
vec3f vec3f::operator+=(vec3f const& op2)
{
	this->x += op2.x;
	this->y += op2.y;
	this->z += op2.z;

	return *this;
}

//-------------------------------------------------
vec3f vec3f::operator-=(vec3f const& op2)
{
	this->x -= op2.x;
	this->y -= op2.y;
	this->z -= op2.z;

	return *this;
}

//--------------------------------------------------
vec3f vec3f::normalize() {

	 GLfloat len = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);

	 this->x /= len;
	 this->y /= len;
	 this->z /= len;

	 return *this;
 }

//--------------------------------------------------
vec3f vec3f::cross(vec3f& op2)
{
	return vec3f(this->y * op2.z - this->z * op2.y, this->z * op2.x - this->x * op2.z, this->x * op2.y - this->y * op2.x);
}

