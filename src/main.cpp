#include "renderer.h"

#ifdef main
#undef main
#endif

int main(int argc, char* argv[]) {
	printf("Starting Memrex...\n");

	initialize();

	MemCardView *view = new MemCardView();
	view->Load(argv[1]);
	view->SetIconSize(64);
	view->InitView(display_width, display_height);

	gameloop(view);

	delete (view);

	deinitialize();

	return 0;
}





