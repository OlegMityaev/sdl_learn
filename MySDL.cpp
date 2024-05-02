#include "MySDL.h"
bool success = true;

SDL::SDL(/*Uint32 flags, bool _fullscreen,*/  SDL_Renderer* _render, int w, int h, int _x, int _y)
{
    mWidth = w;
    mHeight = h;
    x = _x;
    y = _y;
    m_render = _render;
    /*
    window = _window;
    if (window != nullptr)
    {
        fullscreen = _fullscreen;
        if (SDL_Init(flags) != 0)
            throw InitError();
        if (!TTF_WasInit())
        {
            if (TTF_Init() < 0)
            {
                std::cout << "SDL_ttf could not init: " << TTF_GetError() << std::endl;
                success = false;
            }
        }

        if (!_fullscreen)
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
    */
}

SDL::~SDL()
{
    SDL_DestroyRenderer(m_render);
    SDL_Quit();
    TTF_Quit();
    IMG_Quit();
}

void SDL::setRender(SDL_Renderer* _render)
{
    m_render = _render;
}

void SDL::copyRend()
{
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    if (m_render != nullptr)
    {
        SDL_RenderCopy(m_render, m_texture, NULL, &renderQuad);
    }
    else
    {
        std::cout << "Render = nullptr" << std::endl;
    }
}

void SDL::loadTexture(std::string path)
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
        /*
        if (newTexture == nullptr)
        {
            std::cout << "Unable to create texture from " << path.c_str() << " " << SDL_GetError() << std::endl;
        }
        else
        {
            mWidth = static_cast<size_t>(loadedSurface->w);
            mHeight = static_cast<size_t>(loadedSurface->h);
        }*/
        SDL_FreeSurface(loadedSurface);
    }

    m_texture = newTexture;
}

void SDL::rendering()
{
    // Стираем всё белым цветом
    SDL_SetRenderDrawColor(m_render, 255, 255, 255, 255);
    SDL_RenderClear(m_render);
    // Смотрим результат
    if (m_texture != nullptr)
    {
        if (!fullscreen) {
            int w;
            int h;
            SDL_QueryTexture(m_texture, NULL, NULL, &w, &h);
        }
        else
        {
            SDL_QueryTexture(m_texture, NULL, NULL, 0, 0);
        }
    }
    else
    {
        std::cout << "draw error - texture = nullptr" << SDL_Error << std::endl;
    }

    SDL_RenderCopy(m_render, m_texture, NULL, 0);
}

void SDL::SetXY(int _x, int _y)
{
    x = _x;
    y = _y;
}

SDL_Renderer* SDL::getRender()
{
    return m_render;
}
