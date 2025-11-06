#pragma once
#include <SDL3/SDL_render.h>

#include "Classes/Component/SenceComponent/SceneComponent.hpp"
#include "Types/Vec.hpp"


enum class TextureType : int
{
	StaticTexture,
	DynamicTexture
};


class Texture : public Object
{
	std::shared_ptr<SDL_Texture> texture;
	LayerHierarchy layer_hierarchy;
public:
	int w,h;
	Vec2<float> pivot;
	Texture();
	void Construct() override;
	void Copy(Texture& other);
	std::shared_ptr<SDL_Texture> GetTexture() const;
	Vec2<float> GetSize() const;
	void NativeSetRenderLayer(LayerHierarchy new_layer);
	//static void SafeDestroyTexture(SDL_Texture* texture);
	void SetTexture(const std::shared_ptr<SDL_Texture> &new_texture);
};