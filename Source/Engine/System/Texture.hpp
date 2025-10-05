#pragma once
#include <SDL3/SDL_render.h>
#include "Classes/Object.hpp"
#include "Types/Vec.hpp"

struct StaticTexture : public Object
{
	SDL_Texture* texture;
	int w;
	int h;
public:
    StaticTexture();

	explicit StaticTexture(Vec2d<int> size, SDL_Color color = Color_White, bool is_fill = false);
	StaticTexture(SDL_Texture* texture, SDL_TextureAccess texture_mode = SDL_TEXTUREACCESS_STREAMING);
	StaticTexture(StaticTexture&& other);
    ~StaticTexture();
	SDL_Texture* GetTexture() const ;


	SDL_Texture* operator()() const
	{
		return texture;
	}
};

