#pragma once
#include "Texture.hpp"


struct StaticTexture : public Texture
{

	StaticTexture();
	//纯色
	explicit StaticTexture(Vec2<int> size, SDL_Color color = Color_White, bool is_fill = false);

	//用图片加载
	StaticTexture(SDL_Texture* texture, SDL_TextureAccess texture_mode = SDL_TEXTUREACCESS_STREAMING);

	StaticTexture(const StaticTexture& other);

	//拷贝构造
	//StaticTexture(StaticTexture& other);

	//StaticTexture(StaticTexture&& other) noexcept ;
	TextureType GetTextureType() override;
	void ComponentRender() override;
	~StaticTexture() override;
};

