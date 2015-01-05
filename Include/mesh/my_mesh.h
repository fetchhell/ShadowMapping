#pragma once

//#include <file/file.h>

#include "lib3ds\io.h"
#include "lib3ds\vector.h"
#include "lib3ds\file.h"
#include "lib3ds\mesh.h"
#include "lib3ds\chunk.h"
#include "lib3ds\types.h"
#include "lib3ds\atmosphere.h"
#include "lib3ds\matrix.h"
#include "lib3ds\material.h"
#include "lib3ds\mesh.h"

class mesh
{
   public:
   mesh() { m_TotalFaces = 0; }
   bool loadModel(const char* name);
   void GetFaces(Lib3dsFile *f);
   Lib3dsVector* getVertex()  { return m_vertex; }
   Lib3dsVector* getNormals() { return m_normals; } 
   Lib3dsTexel*  getTexCoord() { return m_texcoord; } 
   int getNumFaces() { return m_TotalFaces; }

   private:
   Lib3dsVector* m_normals;
   Lib3dsVector* m_vertex;
   Lib3dsTexel* m_texcoord;
   int m_TotalFaces;
};