#include <png/my_png.h>


size_t png::read(unsigned char *dest, const size_t byteCount) {

	if(fp) 
	  fread(dest, 1, byteCount, fp);

	return byteCount;
}

//-----------------------------------------------------------
 GLenum png::getFormat()
{
   png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);

  switch (color_type)
  {
	  case PNG_COLOR_TYPE_RGB:
		return GL_RGB;
	  case PNG_COLOR_TYPE_RGB_ALPHA:
		return GL_RGBA;
	  case PNG_COLOR_TYPE_GRAY:
		return GL_LUMINANCE;
  }

  return GL_NONE;
}

//-----------------------------------------------------------
ui png::getPitch()
{
  int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
  rowbytes += 3 - ((rowbytes-1) % 4);
  
  return rowbytes;
}

//-----------------------------------------------------------
void png::ReadDataFromInputStream(png_structp png_ptr, png_bytep outBytes,
   png_size_t byteCountToRead)
{
   png_voidp io = png_get_io_ptr(png_ptr);

   fread(outBytes, 1, byteCountToRead, (FILE*)io);
}

//-----------------------------------------------------------
bool png::png_texture_load(const char* name)
{
  if (!name)
    return false;

  fp = fopen(name, "rb");

  if (!fp) return false;

  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(!png_ptr) 
	  return false;

  info_ptr = png_create_info_struct(png_ptr);
  if(!info_ptr)
  {
	  png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
	  return false;
  }

  end_info = png_create_info_struct(png_ptr);
  if(!end_info)
  {
	  png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
	  return false;
  }

  if(setjmp(png_jmpbuf(png_ptr))) 
  {
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     return false;
  }

  	png_set_read_fn(png_ptr, fp, ReadDataFromInputStream);
    png_read_info(png_ptr, info_ptr);

    int bit_depth, color_type;
    png_uint_32 width, height;

    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
        NULL, NULL, NULL);

    png_read_update_info(png_ptr, info_ptr);

	int rowbytes = getPitch();
	png_byte* image_data = new png_byte[rowbytes * height + 15];

	if(!image_data)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		return false;
	}

	png_bytep* row_pointers = new png_bytep[height];  
	if (row_pointers == NULL)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        delete [] image_data;
        return false;
    }

    for (int i = 0; i < height; i++)
        row_pointers[height - 1 - i] = image_data + i * rowbytes;
    
	png_read_image(png_ptr, row_pointers);

	m_format = getFormat();

	// save data
	m_width  = width;
	m_height = height;
	m_depth  = 1;
	m_pitch  = rowbytes;

	m_image_data = new png_byte[rowbytes * height + 15]; 
	memcpy(m_image_data, image_data, rowbytes * height + 15);

	// Free data
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	if(image_data)   delete [] image_data;
	if(row_pointers) delete [] row_pointers;

    return true;
}