#include "Includs.h"
#include "MySDL.h"


int main(int argc, char* argv[])
{
	
	SDL sdl(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	const char* filename = "backscreen.jpg";
	SDL_Texture* texture = sdl.loadTexture(filename);
	sdl.draw(texture);
	return 0;
}

