#pragma once

#include <include/includeAll.h>
#include <libpng/png.h>
#include <zlib/zlib.h>
#include <config/Config.h>
#include <file/my_file.h>

typedef unsigned int ui; 

using namespace std;

class png
{
    public:
	Config* config;
	bool png_texture_load(const char* name);
	png_byte* getImageData() { return m_image_data; }
	GLenum getFormat2() { return m_format;}
	ui getWidth()       { return m_width; }
	ui getHeight()      { return m_height;}
	ui getDepth()       { return m_depth; }
	ui getPitch2()      { return m_pitch; }

	size_t read(unsigned char *dest, const size_t byteCount);

	~png() // png_destroy_read_struct
	{ 
		if(m_image_data) delete [] m_image_data; 
		if(config) delete config;
	} 

	FILE* fp;

    private:
	ui getPitch();
	GLenum getFormat();
    static void ReadDataFromInputStream(png_structp png_ptr, png_bytep outBytes,
                                 png_size_t byteCountToRead);

	png_structp png_ptr;
    png_infop info_ptr;
    png_infop end_info;

	// data
	png_byte* m_image_data;
	GLenum m_format;
	ui m_width, m_height, m_depth, m_pitch;
};
