#pragma once
#include <SDL3/SDL_render.h>

#include "SceneComponent.hpp"
#include "Types/Vec.hpp"

struct Texture : public SceneComponent
{
	Texture()
	{
		name = "Texture";
	}
};

struct StaticTexture : public Texture
{
	SDL_Texture* texture;
	int w;
	int h;
	Vec2<float> pivot;
public:
    StaticTexture();

	explicit StaticTexture(Vec2<int> size, SDL_Color color = Color_White, bool is_fill = false);
	StaticTexture(SDL_Texture* texture, SDL_TextureAccess texture_mode = SDL_TEXTUREACCESS_STREAMING);

	StaticTexture(StaticTexture& other);

	StaticTexture(StaticTexture&& other) noexcept ;



	void SetPivot(Vec2<float> p);

	~StaticTexture();

	void Copy(StaticTexture& other);
	SDL_Texture* GetTexture() const ;
	Vec2<float> GetSize();

	SDL_Texture* operator()() const
	{
		return texture;
	}
	static SDL_Texture* CreateOutLine();
};

struct DynamicTexture : public Texture
{

};
