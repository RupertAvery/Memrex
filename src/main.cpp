#include "platform.h"
#include "SDL.h"
#include <stdio.h>
#include GLHEADER
#include "math.h"
#include "SDL_image.h" 
#include "time.h"
#include "memcardview.h"
#include <sys/timeb.h>

#ifdef main
#undef main
#endif

#ifdef USE_OPENGL_ES
#define glOrtho glOrthof
#endif

int frameCount;
int currentTime;
int previousTime;
int fps;
unsigned int display_width = 0;
unsigned int display_height = 0;
int t = 0;
#define PI 3.14159265

bool get_input(void) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT: return false; //The little X in the window got pressed
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				return false;
			}
			break;
		}
	}
	return true;
}

int getMilliCount(){
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}

//-------------------------------------------------------------------------
// Calculates the frames per second
//-------------------------------------------------------------------------
void calculateFPS()
{
	//  Increase frame count
	frameCount++;

	//  Get the number of milliseconds since glutInit called
	//  (or first call to glutGet(GLUT ELAPSED TIME)).
	currentTime = getMilliCount();

	//  Calculate time passed
	int timeInterval = currentTime - previousTime;

	if (timeInterval > 1000)
	{
		//  calculate the number of frames per second
		fps = frameCount / (timeInterval / 1000.0f);
		//  Set time
		previousTime = currentTime;

		//  Reset frame count
		frameCount = 0;
	}
}

int x[15];
int y[15];
int dx[15];
int dy[15];

void draw(SDL_Window * window, MemCardView *view) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glLoadIdentity();

	//glTranslatef(10.0f, 10.0f, 0.0f);
	int size = 64;
	int padding = 16;

	//for (int i = 0; i < view->getIconCount(); i++)
	//{
	//	if ((i > 0) && (i % 3 == 0))
	//	{
	//		//glTranslatei( -2.5f * 3, -2.5f, 0.0f ); 
	//	}
	//	view->DrawIcon(i);
	//	glTranslatef((float)(size + padding), 0.0f, 0.0f);
	//}
	for (int i = 0; i < view->getIconCount(); i++)
	{
		x[i] += dx[i];
		if (x[i] < 0) dx[i] = -dx[i];
		if (x[i] + 64 > display_width) dx[i] = -dx[i];
		y[i] += dy[i];

		if (y[i] < 0) dy[i] = -dy[i];
		if (y[i] + 64 > display_height) dy[i] = -dy[i];
	}

	//glTranslatef(display_width / 2, display_height / 2, 0.0f);

	for (int i = 0; i < view->getIconCount(); i++)
	{
		glPushMatrix();
		glTranslatef(x[i], y[i], 0.0f);
		view->DrawIcon(i);
		glPopMatrix();
	}

	//t++;
	


	if (frameCount % 11 == 0)
	{
		view->AnimateIcons();
	}

	SDL_GL_SwapWindow(window);
}




int main(int argc, char* argv[]) {
	printf("Starting Memrex...\n");



	SDL_Window *window;                    // Declare a pointer
	SDL_GLContext maincontext;
	SDL_Init(SDL_INIT_EVERYTHING);              // Initialize SDL2

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// multisample anti-aliasing
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);

#ifdef USE_OPENGL_ES
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
#endif

	SDL_DisplayMode dispMode;
	SDL_GetDesktopDisplayMode(0, &dispMode);

	if (display_width == 0)
		display_width = dispMode.w;

	if (display_height == 0)
		display_height = dispMode.h;

	// Create an application window with the following settings:
	window = SDL_CreateWindow(
		"Memrex",                  // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		display_width,                               // width, in pixels
		display_height,                               // height, in pixels
		SDL_WINDOW_OPENGL
		//| SDL_WINDOW_FULLSCREEN    // flags - see below
		);

	// Check that the window was successfully made
	if (window == NULL) {
		// In the event that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	glEnable(GL_DEPTH_TEST);
	maincontext = SDL_GL_CreateContext(window);

	if (SDL_GL_SetSwapInterval(-1) != 0 && SDL_GL_SetSwapInterval(1) != 0)
	{
		printf("Failed to enbale VSync\n");
	}

	MemCardView *view = new MemCardView();
	view->Load(argv[1]);
	view->SetIconSize(64);

	glViewport(0, 0, display_width, display_height);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, display_width, display_height, 0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	//glClearColor(0.4f, 1.0f, 1.0f, 0.0f);

	srand(time(NULL));

	for (int i = 0; i < view->getIconCount(); i++)
	{
		printf("%d", rand() % 2);
		dx[i] = ((rand() % 2) == 1) ? 1 : -1;
		dy[i] = ((rand() % 2) == 1) ? 1 : -1;
		x[i] = rand() % (display_width - 64);
		y[i] = rand() % (display_height - 64);
	}


	while (true) {
		if (!get_input()) break;
		draw(window, view);
		calculateFPS();
	}

	delete (view);


	SDL_GL_DeleteContext(maincontext);

	// Close and destroy the window
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();
	return 0;

}





