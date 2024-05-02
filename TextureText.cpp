#include "TextureText.h"

TextureText::TextureText() {}

void TextureText::setRenderer(SDL_Renderer* apRenderer)
{
	mp_renderer = apRenderer;
}

bool TextureText::loadFont(std::string path, int size)
{
	free();

	mp_font = TTF_OpenFont(path.c_str(), size);
	if (mp_font == nullptr)
	{
		std::cout << "Failed to load font. SDL_ttf error: " << TTF_GetError() << std::endl;
	}
	return mp_font != nullptr;
}

void TextureText::free()
{
	if (mp_font != nullptr)
	{
		TTF_CloseFont(mp_font);
		mp_font = nullptr;
	}
	if (mp_texture != nullptr)
	{
		SDL_DestroyTexture(mp_texture);
		mp_texture = nullptr;
		mWidth = 0;
		mHeight = 0;
	}
}

void TextureText::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	m_color.r = red;
	m_color.g = green;
	m_color.b = blue;
}

void TextureText::setText(std::string text, Uint8 wrapLength)
{
	m_text = text;

	SDL_Surface* textSurface = TTF_RenderText_Solid_Wrapped(mp_font, m_text.c_str(), m_color, wrapLength);
	if (textSurface != nullptr)
	{
		mp_texture = SDL_CreateTextureFromSurface(mp_renderer, textSurface);
		if (mp_texture != nullptr)
		{
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}
}

void TextureText::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(mp_texture, alpha);
}

void TextureText::render(int x, int y,
	SDL_Rect* clip,
	double angle,
	SDL_Point* center,
	SDL_RendererFlip flip)
{
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	if (clip != nullptr)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	//SDL_SetRenderDrawColor(mp_renderer, 255, 255, 255, 255);
	//SDL_RenderClear(mp_renderer);
	//SDL_RenderCopy(mp_renderer, mp_texture, clip, &renderQuad);
	SDL_RenderCopyEx(mp_renderer, mp_texture, clip, &renderQuad, angle, center, flip);
	//SDL_RenderPresent(mp_renderer);
	
}

