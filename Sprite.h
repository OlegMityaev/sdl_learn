#pragma once
#include "Includs.h"

class Sprite
{
protected:
	size_t mFrameCount;
	size_t mWidth;
	size_t mHeight;
	SDL_Texture* mTexture;
	SDL_Renderer* render;
	std::vector<SDL_Rect> mvRects;
public:
	Sprite();
	Sprite(SDL_Renderer*);
	~Sprite();

	bool LoadFromFile(std::string path, size_t aFrameCount);

	void SetRenderer(SDL_Renderer* _render)
	{
		render = _render;
	}

	void renderer(int x, int y, size_t aFrameNumber);

	size_t getWidth();
	size_t getHeight();
	size_t frameCount();

	void free();
};