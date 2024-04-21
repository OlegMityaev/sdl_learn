#pragma once
#include "Includs.h"
#include "InitError.h"

class SDL
{
protected:
    SDL_Window* window;
    SDL_Renderer* render;
public:
    SDL(Uint32 flags = 0, bool fullscreen = false, int w = 1000, int h = 1000);
    virtual ~SDL();
    void draw(SDL_Texture* texture);
    SDL_Texture* loadTexture(std::string filename);
    SDL_Renderer* getRender();
    SDL_Window* getWindow();
    friend class Sprite;
};