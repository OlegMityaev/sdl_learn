#include "LButton.h"

LButton::LButton(SDL_Renderer* _render, int w, int h, int _x, int _y)
{
    mPosition.x = _x;
    mPosition.y = _y;
    m_texture = nullptr;

    m_render = _render;

    mWidthRend = w;
    mHeightRend = h;
    x = _x;
    y = _y;
    m_render = _render;

    mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition(int x, int y)
{
    mPosition.x = x;
    mPosition.y = y;
}

void LButton::setRender(SDL_Renderer* _render)
{
    m_render = _render;
}

void LButton::copyRend()
{
    SDL_Rect renderQuad = { x, y, static_cast<int>(mWidthRend), static_cast<int>(mHeightRend) };
    if (m_render != nullptr)
    {
        SDL_RenderCopy(m_render, m_texture, &mvRects[mCurrentSprite], &renderQuad);
    }
    else
    {
        std::cout << "Render = nullptr" << std::endl;
    } 
}

void LButton::loadTexture(std::string path, size_t aFrameCount)
{
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    SDL_Texture* newTexture = nullptr;

    if (loadedSurface == nullptr)
    {
        std::cout << "Unable to load image " << path.c_str() << IMG_GetError() << std::endl;
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(m_render, loadedSurface);
        
        if (newTexture == nullptr)
        {
            std::cout << "Unable to create texture from " << path.c_str() << " " << SDL_GetError() << std::endl;
        }
        else
        {
            mWidth = static_cast<size_t>(loadedSurface->w) / aFrameCount;
            mHeight = static_cast<size_t>(loadedSurface->h);
            mFrameCount = aFrameCount;
            mvRects.resize(mFrameCount);
        }

        int x = 0;

        for (auto& r : mvRects)
        {
            r.x = x;
            r.y = 0;
            r.w = static_cast<int>(mWidth);
            r.h = static_cast<int>(mHeight);

            x += mWidth;
        }
        SDL_FreeSurface(loadedSurface);
    }

    m_texture = newTexture;
}

void LButton::ZeroTrueClicked()
{
    true_clicked = 0;
}

void LButton::SetXY(int _x, int _y)
{
    x = _x;
    y = _y;
}

SDL_Renderer* LButton::getRender()
{
    return m_render;
}

void LButton::handleEvent(SDL_Event* e)
{
    //If mouse event happened
    if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState(&x, &y);
        bool inside = false;

        if (x > mPosition.x && x < mPosition.x + mWidthRend && y > mPosition.y && y < mPosition.y + mHeightRend)
        {
            inside = true;
        }
        if (!inside)
        {
            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }
        //Mouse is inside button

        else
        {
            //Set mouse over sprite
            switch (e->type)
            {
            case SDL_MOUSEMOTION:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                choosenFrame = 0;
                break;

            case SDL_MOUSEBUTTONDOWN:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                choosenFrame = 1;
                true_clicked = 1;
                break;

            case SDL_MOUSEBUTTONUP:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
                choosenFrame = 2;
                break;
            }
        }
    }
}

int LButton::getTrueClicked()
{
    return true_clicked;
}
