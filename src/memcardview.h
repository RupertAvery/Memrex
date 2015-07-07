#ifndef __MEMCARDVIEW_H__
#define __MEMCARDVIEW_H__

#include "platform.h"
#include "SDL.h"
#include GLHEADER
#include "memcard.h"
#include <sys/timeb.h>
#include <math.h>
#include <time.h>

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

	int display_width;
	int display_height;

	int x[15];
	int y[15];
	int dx[15];
	int dy[15];

public:
	int getIconCount();
	void SetIconSize(int iconSize);
	void DrawIcon(int index);
	void AnimateIcons();
	void InitView(int display_width, int display_height);
	void Load(char * path);
	void Render(SDL_Window * window);
	~MemCardView();
};

#endif //__MEMCARDVIEW_H__
