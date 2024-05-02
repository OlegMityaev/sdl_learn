#pragma once
#include "Includs.h"
#include "InitError.h"

class SDL
{
protected:
    int x, y;
    size_t mWidth, mHeight;
    //SDL_Window* window;
    SDL_Renderer* m_render;
    SDL_Texture* m_texture;
    bool fullscreen;
public:
    SDL(/*Uint32 flags = 0, bool _fullscreen = false,*/SDL_Renderer* _render = nullptr, int w = 1920, int h = 1080,  int _x = 0, int _y = 0);
    ~SDL();
    void setRender(SDL_Renderer* _render);
    void copyRend();
    void rendering();
    void SetXY(int x, int y);
    void loadTexture(std::string filename);
    SDL_Renderer* getRender();
    friend class Sprite;
    friend class LButton;
};