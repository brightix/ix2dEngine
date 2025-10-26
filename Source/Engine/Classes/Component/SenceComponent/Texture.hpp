#pragma once
#include <SDL3/SDL_render.h>

#include "SceneComponent.hpp"
#include "Types/Vec.hpp"


enum class TextureType : int
{
	StaticTexture,
	DynamicTexture
};


class Texture : public SceneComponent
{
protected:
	std::shared_ptr<SDL_Texture> in_texture;
public:
	float w;
	float h;
	Vec2<float> pivot;
	Texture();

	void Construct() override;

	void SetPivot(Vec2<float> p);
	void Copy(Texture& other);


	std::shared_ptr<SDL_Texture> GetTexture() const;
	Vec2<float> GetSize() const;

	virtual TextureType GetTextureType();
	static void SafeDestroyTexture(SDL_Texture* texture);
	void SetNewTexture(std::shared_ptr<SDL_Texture> new_texture);
};