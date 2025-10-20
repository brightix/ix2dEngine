#pragma once
#include "Texture.hpp"


struct StaticTexture : public Texture
{
	//Vec2<float> transform;
	StaticTexture();
	//纯色
	//explicit StaticTexture(Vec2<int> size, SDL_Color color = Color_White, bool is_fill = false);

	//用图片加载
//浅
	StaticTexture(const StaticTexture& other);

	//拷贝构造
	//StaticTexture(StaticTexture& other);

	//StaticTexture(StaticTexture&& other) noexcept ;
	void Construct() override;
	TextureType GetTextureType() override;
	void ComponentRender() override;

	//void LoadDefaultTexture(Vec2<int> size, SDL_Color color = {255,255,255,255}, bool is_fill = false);

	void OfferRenderData(std::vector<RenderData>& data) override;
};

