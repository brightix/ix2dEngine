#pragma once
#include <SDL3/SDL_render.h>
#include "Classes/Object.h"
#include "Types/Vec.hpp"

struct StaticTexture : public Object
{
	SDL_Texture* texture;
	float w;
	float h;
public:
    StaticTexture();
	StaticTexture(Vec2d<float> size, SDL_Color color);
    ~StaticTexture()= default;
	SDL_Texture* operator()() const
	{
		return texture;
	}
};

