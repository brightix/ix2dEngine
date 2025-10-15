#pragma once
#include <SDL3/SDL_render.h>

#include "SceneComponent.hpp"
#include "Types/FRect.hpp"
#include "Types/Vec.hpp"

enum class TextureType : int
{
	StaticTexture,
	DynamicTexture
};

struct Texture : public SceneComponent
{
	SDL_Texture* texture;
	int w;
	int h;
	Vec2<float> pivot;
	Texture() : texture(nullptr), w(0), h(0)
	{
		name = "Texture";
	}
	Texture(SDL_Texture* texture, SDL_TextureAccess texture_mode = SDL_TEXTUREACCESS_STREAMING);

	void SetPivot(Vec2<float> p);

	static SDL_Texture* CreateOutLineTexture(const FRect& rect);

	static SDL_Texture* CreateFilledTexture(const FRect& rect);

	void Copy(Texture& other);
	SDL_Texture* GetTexture() const;
	Vec2<float> GetSize() const;

	virtual TextureType GetTextureType()= 0;
};