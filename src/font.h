#ifndef __FONT_H__
#define __FONT_H__
#define GL_GLEXT_PROTOTYPES

#include <ft2build.h>
#include FT_FREETYPE_H
#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glext.h"


 

int init_freetype();
void render_text(const char *text, float x, float y, float sx, float sy) ;

#endif //__FONT_H__
