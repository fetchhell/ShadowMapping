#pragma once

#include <include/includeAll.h>

class Config
{
   public:
    Config(GLfloat min_filter, GLfloat mag_filter, GLfloat wrap_s, GLfloat wrap_t, bool generate_mipmaps):min_filter(min_filter), mag_filter(mag_filter), 
	                                                                                              wrap_s(wrap_s), wrap_t(wrap_t), generate_mipmaps(generate_mipmaps){};

   GLfloat getMinFilterMode() { return min_filter; }
   GLfloat getMagFilterMode() { return mag_filter; }
	       
   GLfloat getWrapsMode() { return wrap_s; } 
   GLfloat getWraptMode() { return wrap_t; }

   bool generateMipmaps() { return generate_mipmaps; }

   private:
   GLfloat min_filter;
   GLfloat mag_filter;

   GLfloat wrap_s;
   GLfloat wrap_t;

   bool generate_mipmaps;
};