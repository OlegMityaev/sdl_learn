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
        IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    }
}

SDL::~SDL()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(render);
    SDL_Quit();
}

SDL_Texture* SDL:: loadTexture(std::string filename)
{ 
    SDL_Texture* texture;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);
    texture = IMG_LoadTexture(render, filename.c_str());
    return texture;
}

void SDL::draw(SDL_Texture* texture)
{
    int w;
    int h;
    // Стираем всё белым цветом
    //SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    SDL_RenderClear(render);
    // Смотрим результат

    SDL_QueryTexture(texture, NULL, NULL, &w, &h);

    SDL_RenderCopy(render, texture, NULL, 0);
    SDL_RenderPresent(render);
    SDL_Delay(5000);
}

SDL_Renderer* SDL::getRender()
{
    return render;
}

SDL_Window* SDL::getWindow()
{
    return window;
}