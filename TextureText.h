#pragma once
#include "Includs.h"

class TextureText
{
private:
	std::string m_text;
	SDL_Color m_color;
	int mWidth = 0;
	int mHeight = 0;
	Uint8 wrapLentgh = 1080;

	SDL_Renderer* mp_renderer = nullptr;
	SDL_Texture* mp_texture;
	TTF_Font* mp_font = nullptr;
public:
	TextureText();
	void setRenderer(SDL_Renderer* apRenderer);
	bool loadFont(std::string path, int size = 24);
	void free();
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	void setAlpha(Uint8 alpha);
	void render(int x, int y,
		SDL_Rect* clip = nullptr,
		double angle = 0.0,
		SDL_Point* center = nullptr,
		SDL_RendererFlip flip = SDL_FLIP_NONE);
	void setText(std::string text, Uint8 wrapLength);
};