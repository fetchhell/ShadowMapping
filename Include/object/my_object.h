#pragma once
#include <mesh/my_mesh.h>
#include <matrix/nv_matrix.h>
#include <vector3f/vec.h>

class object
{
	 friend class Scene;
	 friend class Shader;

     public:

     object();

	 void scaleObject(vec3f scaleVector);
	 void translateObject(vec3f translateVector);

	 /* Model matrix */
	 float m_model[4][4];

     private:
	 mesh m_mesh;

	 /* VBO for vertices, normals, texture coordinates */
	 ui VBO[3];

     /* UBO for matrix */
     ui UBO[1];
};