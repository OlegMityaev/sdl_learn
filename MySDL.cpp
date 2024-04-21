#include "MySDL.h"

SDL::SDL(Uint32 flags, bool fullscreen, int w, int h)
{
    if (SDL_Init(flags) != 0)
        throw InitError();
    if (!fullscreen)
    {
        if (SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_SHOWN, &window, &render) != 0)
            throw InitError();
        SDL_SetWindowTitle(window, "Capybara Ross");
    }
    else
    {
        SDL_DisplayMode displayMode;
        if (SDL_GetDesktopDisplayMode(0, &displayMode))
            throw InitError();
        window = SDL_CreateWindow("Capybara Ross", 0, 0, displayMode.w, displayMode.h, SDL_WINDOW_BORDERLESS);
        if (window == nullptr)
            throw InitError();
        render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (window == nullptr)
            throw InitError();
    }
}



SDL::~SDL()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(render);
    SDL_Quit();
}
