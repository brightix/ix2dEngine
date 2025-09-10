#pragma once
#include <SDL3/SDL_render.h>
#include "Classes/Object.h"
#include "Types/Vec.hpp"

struct Texture : public Object
{
	SDL_Texture* texture;
public:
    Texture();
	Texture(Vec2d<float> size, SDL_Color color);
    ~Texture()= default;
	SDL_Texture* operator()() const
	{
		return texture;
	}
};

