#ifndef __MEMCARDVIEW_H__
#define __MEMCARDVIEW_H__

#include "memcard.h"
#include "platform.h"

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
};

class MemCardView {
	ICONVIEW icon[15];
	int iconCount;
	void setPixel(unsigned char* pixels, int pointer, unsigned char index, char* palette);
    void BuildTexture(GLuint *texture, ICON* icon, char* palette);
	char* SJIStoASCII(char* sjis);


public:
	int getIconCount();
	void DrawIcon(int index, int frame, int size);
	void Load(char * path);
	~MemCardView();
};

#endif //__MEMCARDVIEW_H__
