#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <stdio.h>

#include "platform.h"
#include "SDL.h"
#include "SDL_image.h" 
#include GLHEADER
#include "font.h"
#include "memcardview.h"

#ifdef USE_OPENGL_ES
#define glOrtho glOrthof
#endif

#define PI 3.14159265


extern int frameCount;
extern int currentTime;
extern int previousTime;
extern int fps;

extern unsigned int display_width;
extern unsigned int display_height;
extern int t;

extern SDL_Window *window;                    // Declare a pointer
extern SDL_GLContext maincontext;

void initialize();
bool get_input(void);
int getMilliCount();
void calculateFPS();
void draw(MemCardView *view);
void gameloop(MemCardView *view);
void deinitialize();

#endif //__RENDERER_H__

