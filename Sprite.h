#pragma once
#include "Includs.h"

class Sprite
{
protected:
	size_t mFrameCount = 0;
	size_t mWidth = 0;
	size_t mHeight = 0;
	SDL_Texture* mTexture = nullptr;
	SDL_Renderer* render = nullptr;
	Mix_Chunk* gMusic = nullptr;
	std::vector<SDL_Rect> mvRects;
public:
	Sprite();
	Sprite(SDL_Renderer*);
	~Sprite();

	bool LoadFromFile(std::string path, size_t aFrameCount);
	bool loadAudio(std::string path_audio);

	Mix_Chunk* getAudio()
	{
		return gMusic;
	}

	SDL_Renderer* getRender()
	{
		return render;
	}

	void SetRenderer(SDL_Renderer* _render)
	{
		render = render;
	}

	void renderer(int x, int y, size_t aFrameNumber);

	size_t getWidth();
	size_t getHeight();
	size_t frameCount();

	void free();
};