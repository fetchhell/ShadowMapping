#include <mesh/my_mesh.h>
#include <stdlib.h>
#include <string.h>

//-----------------------------------------------------------
long MySeek(void* stream, long offset, Lib3dsIoSeek type) {

  int o;
  switch (type) {
    case LIB3DS_SEEK_SET:
      o = SEEK_SET;
      break;
    case LIB3DS_SEEK_CUR:
      o = SEEK_CUR;
      break;
    case LIB3DS_SEEK_END:
      o = SEEK_END;
      break;
    default:
      ASSERT(0);
      return(0);
  }
	return fseek((FILE*)stream, offset, o);
}

//-----------------------------------------------------------
size_t MyRead(void *self, void *buffer, size_t size) {

	return fread(buffer, 1, size, (FILE*) self);
}

//-----------------------------------------------------------
long MyTell (void *self) {

	return ftell((FILE*)self);
}

//-----------------------------------------------------------
void mesh::GetFaces(Lib3dsFile *f)
{
     Lib3dsMesh* mesh;	
     for(mesh = f->meshes;mesh != NULL; mesh = mesh->next)   
         m_TotalFaces += mesh->faces;    
}

//-----------------------------------------------------------
bool mesh::loadModel(const char* name)
{
	FILE* nv_file = fopen(name, "rb");
	if(!nv_file) return false;

	Lib3dsIo io;
	memset(&io, 0, sizeof(io));
	
	io.self = nv_file;
	io.seek_func = MySeek;
	io.tell_func = MyTell;
	io.read_func = MyRead;
	io.write_func = NULL;
	
	Lib3dsFile* file3ds = lib3ds_file_new();
	if(!file3ds) return false;

	int result = lib3ds_file_read(file3ds, &io);
	if(!result) return false;
	
	GetFaces(file3ds);
	m_vertex   = new Lib3dsVector[m_TotalFaces * 3];
    m_normals  = new Lib3dsVector[m_TotalFaces * 3];
	m_texcoord = new Lib3dsTexel[m_TotalFaces * 3];

    Lib3dsMesh * mesh;
	unsigned int FinishedFaces = 0;
	for(mesh = file3ds->meshes;mesh != NULL;mesh = mesh->next)
     {
          lib3ds_mesh_calculate_normals(mesh, &m_normals[FinishedFaces*3]);
          for(unsigned int cur_face = 0; cur_face < mesh->faces;cur_face++)
          {
               Lib3dsFace * face = &mesh->faceL[cur_face];
               for(unsigned int i = 0;i < 3;i++)
               {
				   if(mesh->texels)
					   memcpy(&m_texcoord[FinishedFaces*3 + i], mesh->texelL[face->points[ i ]], sizeof(Lib3dsTexel));

                   memcpy(&m_vertex[FinishedFaces*3 + i], mesh->pointL[face->points[ i ]].pos, sizeof(Lib3dsVector));
               }
               FinishedFaces++;
          }
      }

	return true;
}