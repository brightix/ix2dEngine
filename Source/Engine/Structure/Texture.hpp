#pragma once
#include <SDL3/SDL_render.h>
#include "Classes/Object.hpp"
#include "Types/Vec.hpp"

struct Texture : public Object
{

};

struct StaticTexture : public Texture
{
	SDL_Texture* texture;
	int w;
	int h;
	Vec2d<float> pivot;
public:
    StaticTexture();

	explicit StaticTexture(Vec2d<int> size, SDL_Color color = Color_White, bool is_fill = false);
	StaticTexture(SDL_Texture* texture, SDL_TextureAccess texture_mode = SDL_TEXTUREACCESS_STREAMING);

	StaticTexture(StaticTexture& other);

	StaticTexture(StaticTexture&& other) noexcept ;



	void SetPivot(Vec2d<float> p);

	~StaticTexture();

	void Copy(StaticTexture& other);
	SDL_Texture* GetTexture() const ;


	SDL_Texture* operator()() const
	{
		return texture;
	}
};

struct DynamicTexture : public Texture
{

};
