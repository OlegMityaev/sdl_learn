#pragma once
#include "Includs.h"
#include "InitError.h"

class SDL
{
    SDL_Window* window;
    SDL_Renderer* render;
public:
    SDL(Uint32 flags = 0, bool fullscreen = false, int w = 1024, int h = 768);
    virtual ~SDL();
    void draw(SDL_Texture* texture);
    SDL_Texture* loadTexture(const char* filename);
};