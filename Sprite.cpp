#include "Sprite.h"

Sprite::Sprite()
{
	mTexture = nullptr;
	render = nullptr;
}

Sprite::Sprite(SDL_Renderer* _render)
{
	render = _render;
}

Sprite::~Sprite()
{
	SDL_DestroyRenderer(render);
	SDL_Quit();
}

bool Sprite::LoadFromFile(std::string path, size_t aFrameCount)
{
	SDL_RenderClear(render);
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	SDL_Texture* newTexture = nullptr;

	if (loadedSurface == nullptr)
	{
		std::cout << "Unable to load image " << path.c_str() << IMG_GetError() << std::endl;
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(render, loadedSurface);
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
	
	mTexture = newTexture;
	return mTexture != nullptr;
}

void Sprite::renderer(int x, int y, size_t aFrameNumber)
{
	SDL_Rect renderQuad = { x, y, static_cast<int>(mWidth), static_cast<int>(mHeight) };
	//render to screen
	SDL_RenderCopy(render, mTexture, &mvRects[aFrameNumber], &renderQuad);
}

void Sprite::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

size_t Sprite::getWidth()
{
	return mWidth;
}

size_t Sprite::getHeight()
{
	return mHeight;
}

size_t Sprite::frameCount()
{
	return mFrameCount;
}