#ifndef __MEMCARDVIEW_H__
#define __MEMCARDVIEW_H__

#include "platform.h"
#include GLHEADER
#include "memcard.h"

#ifdef USE_OPENGL_ES
#define	GLNUM GLshort
#define	GLTYPE GL_SHORT
#else
#define	GLNUM GLint
#define	GLTYPE GL_INT
#endif



struct ICONVIEW {
	GLuint texture[3];
	int frameCount;
	int currentFrame;
};

class MemCardView {
	ICONVIEW icon[15];
	int iconCount;
	void setPixel(unsigned char* pixels, int pointer, unsigned char index, char* palette);
	void BuildTexture(GLuint *texture, ICON* icon, char* palette);
	char* SJIStoASCII(char* sjis);
	GLNUM vertices[4][2];

public:
	int getIconCount();
	void SetIconSize(int iconSize);
	void DrawIcon(int index);
	void AnimateIcons();
	void Load(char * path);
	~MemCardView();
};

#endif //__MEMCARDVIEW_H__
