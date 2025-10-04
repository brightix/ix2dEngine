#pragma once
#include <SDL3/SDL_render.h>
#include "Classes/Object.hpp"
#include "Types/Vec.hpp"

struct StaticTexture : public Object
{
	SDL_Texture* texture;
	float w;
	float h;
public:
    StaticTexture();

	StaticTexture(Vec2d<float> size, SDL_Color color = Color_White, bool is_fill = false);
	StaticTexture(SDL_Texture* texture);
    ~StaticTexture();
	SDL_Texture* GetTexture() const ;


	SDL_Texture* operator()() const
	{
		return texture;
	}
};

