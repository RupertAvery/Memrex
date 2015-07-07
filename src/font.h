#ifndef __FONT_H__
#define __FONT_H__
#define GL_GLEXT_PROTOTYPES

#include <ft2build.h>
#include FT_FREETYPE_H
#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glext.h"



class TextDrawer
{
public:
	void init();
	void destroy();
	void renderText(const char *_pText, float x, float y, float sx, float sy) const;
	static TextDrawer & get();

private:
	TextDrawer();
	TextDrawer(const TextDrawer &);
	friend class OGLRender;
	struct Atlas * m_pAtlas;
	GLuint m_program;
	GLint m_uTex;
	GLint m_uColor;
	GLuint m_vbo;
};

#endif //__FONT_H__
