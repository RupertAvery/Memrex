#include "renderer.h"

unsigned int display_width = 0;
unsigned int display_height = 0;
int t = 0;

int x[15];
int y[15];
int dx[15];
int dy[15];

int frameCount;
int currentTime;
int previousTime;
int fps;


SDL_Window *window;                    // Declare a pointer
SDL_GLContext maincontext;
TextDrawer &textDrawer = TextDrawer::get();

void initialize() {

	SDL_Init(SDL_INIT_EVERYTHING);              // Initialize SDL2
	textDrawer.init();

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
		return;
	}

	glEnable(GL_DEPTH_TEST);
	maincontext = SDL_GL_CreateContext(window);

	if (SDL_GL_SetSwapInterval(-1) != 0 && SDL_GL_SetSwapInterval(1) != 0)
	{
		printf("Failed to enbale VSync\n");
	}

	glViewport(0, 0, display_width, display_height);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, display_width, display_height, 0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	//glClearColor(0.4f, 1.0f, 1.0f, 0.0f);

}

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

void draw(MemCardView *view) {
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	view->Render(window);

	  GLfloat black[4] = {255, 255, 255, 1};
	  //glUniform4f(uniform_color, 1, black);

	  float sx = 20.0 / display_width;
	  float sy = 20.0 / display_height;

	glLoadIdentity();
	textDrawer.renderText("Hello World!",10,10,sx,sy);
	SDL_GL_SwapWindow(window);
}

void update(MemCardView *view) {
	if (frameCount % 11 == 0)
	{
		view->AnimateIcons();
	}
}

void gameloop(MemCardView* view) {
	while (true) {
		if (!get_input()) break;
		update(view);
		draw(view);
		calculateFPS();
	}
}

void deinitialize() {
	SDL_GL_DeleteContext(maincontext);

	// Close and destroy the window
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();
}


