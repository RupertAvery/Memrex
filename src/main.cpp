#include "SDL.h"
#include <stdio.h>

#include "GL/gl.h"
#include "GL/glu.h"
 
#include "SDL_image.h" //Needed for IMG_Load.  If you want to use bitmaps (SDL_LoadBMP), it appears to not be necessary
#include "SDL_opengl.h"

#include "memcardview.h"


bool get_input(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: return false; //The little X in the window got pressed
            case SDL_KEYDOWN:
                if (event.key.keysym.sym==SDLK_ESCAPE) {
                    return false;
                }
                break;
        }
    }
    return true;
}


void draw(SDL_Window * window, MemCardView *view) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 
    glViewport(0,0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluOrtho2D(0.0, 800.0, -600.0, 0.0);
//    gluPerspective(45.0, (double)(800)/(double)(600), 0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
 
    glLoadIdentity();
    //gluLookAt(2.0,3.0,4.0, 0.0,0.0,0.0, 0.0,1.0,0.0);
 
	glTranslatef( 48.0f, -48.0f, 0.0f ); 
	glScalef( 32.0f, 32.0f, 32.0f); 

	for(int i=0; i < view->getIconCount(); i++)
	{
		if((i > 0) && (i % 3 == 0))
		{
			glTranslatef( -2.5f * 3, -2.5f, 0.0f ); 
		}
		view->DrawIcon(i);
		glTranslatef( 2.5f, 0.0f, 0.0f ); 
	}

    SDL_GL_SwapWindow(window);
}


int main(int argc, char* argv[]) {
	printf("Starting Memrex...\n");


    SDL_Window *window;                    // Declare a pointer
SDL_GLContext maincontext; 
    SDL_Init(SDL_INIT_EVERYTHING);              // Initialize SDL2

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        800,                               // width, in pixels
        600,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // Check that the window was successfully made
    if (window == NULL) {
        // In the event that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    glEnable(GL_DEPTH_TEST);
  	maincontext = SDL_GL_CreateContext(window);

	MemCardView *view = new MemCardView();
	view->Load(argv[1]);
 
    while (true) {
        if (!get_input()) break;
        draw(window, view);
    }

	delete (view);


    SDL_GL_DeleteContext(maincontext);

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;

}





