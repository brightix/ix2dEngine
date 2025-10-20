#pragma once
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>

struct SDLTextureDeleter
{
	void operator()(SDL_Texture* t) const
	{
		if (t) SDL_DestroyTexture(t);
	}
};

struct SDLSurfaceDeleter
{
	void operator()(SDL_Surface* t) const
	{
		if (t) SDL_DestroySurface(t);
	}
};

struct SDLTTFDeleter
{
	void operator()(TTF_Font* t) const
	{
		if(t) {
			TTF_CloseFont(t);
		}
	}
};