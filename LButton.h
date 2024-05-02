#pragma once
#include "Includs.h"
#include "MySDL.h"

enum LButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2
};

class LButton : public SDL
{
private:
    int x, y;
    size_t mFrameCount = 0;
    size_t mWidth, mHeight, mWidthRend, mHeightRend;
    //SDL_Window* window;
    SDL_Renderer* m_render;
    SDL_Texture* m_texture = nullptr;
    //Верхнее левое положение 
    SDL_Point mPosition;
    std::vector<SDL_Rect> mvRects;
    //Используемый в данный момент глобальный спрайт 
    LButtonSprite mCurrentSprite;
    int choosenFrame = 0;
    int true_clicked = 0;
public:
    //Инициализирует внутренние переменные 
    LButton(SDL_Renderer* _render = nullptr, int w = 1920, int h = 1080, int _x = 0, int _y = 0);
    //Устанавливаем верхнюю левую позицию 
    void setPosition(int x, int y);
    void setRender(SDL_Renderer* _render);
    void copyRend();
    void ZeroTrueClicked();
    void SetXY(int x, int y);
    void loadTexture(std::string filename, size_t frameCount);
    SDL_Renderer* getRender();
    int getTrueClicked();
    //Обрабатывает событие мыши 
    void handleEvent(SDL_Event* e);

};