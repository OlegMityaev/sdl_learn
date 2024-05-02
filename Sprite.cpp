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
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
	if (gMusic != NULL)
	{
		//Free the sound effects
		Mix_FreeChunk(gMusic);
		gMusic = NULL;

		//Quit SDL subsystems
		Mix_Quit();
	}
	SDL_DestroyRenderer(render);
	SDL_Quit();
	Mix_CloseAudio();
	if(TTF_WasInit())
		TTF_Quit();
	IMG_Quit();
}

bool Sprite::LoadFromFile(std::string path, size_t aFrameCount)
{
	//SDL_RenderClear(render);
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

bool Sprite::loadAudio(std::string path_audio)
{
	bool success = true;

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
		success = false;
	}

	//Load music
	gMusic = Mix_LoadWAV(path_audio.c_str());
	if (gMusic == NULL)
	{
		std::cout << "Failed to load beat music! SDL_mixer Error: " << Mix_GetError() << std::endl;
		success = false;
	}

	return success;
}

void Sprite::renderer(int x, int y, size_t aFrameNumber)
{
	SDL_Rect renderQuad = { x, y, static_cast<int>(mWidth), static_cast<int>(mHeight) };
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
	if (gMusic != NULL)
	{
		//Free the sound effects
		gMusic = NULL;
		Mix_FreeChunk(gMusic);
		

		//Quit SDL subsystems
		Mix_Quit();
	}
	Mix_CloseAudio();
	SDL_RenderClear(render);
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