#pragma once

#include <include/includeAll.h>
#include <math.h>

class vec3f
{ 
   public:
   vec3f(float x, float y, float z):x(x),y(y),z(z){};
   vec3f(vec3f const& v);
   vec3f(){};

   vec3f operator*(float x);
   vec3f operator*(vec3f const& op2);
   vec3f operator+(vec3f const& op2);
   vec3f operator-(vec3f const& op2);
   vec3f operator=(vec3f const& op2);
   vec3f operator+=(vec3f const& op2);
   vec3f operator-=(vec3f const& op2);

   vec3f normalize();
   vec3f cross(vec3f& op2);

   float x, y, z;
};