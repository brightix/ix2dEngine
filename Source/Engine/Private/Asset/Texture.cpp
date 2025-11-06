#include "../../Asset/Texture.hpp"

#include "Classes/Core/GameEngine.hpp"
#include "Utilities/FuncLib/Deleter.hpp"


Texture::Texture()
{
	//先引用默认渲染图
	CNAME;
	texture = GetDefaultTexture();
	w = texture->w;
	h = texture->h;
}

void Texture::Construct()
{
	Object::Construct();
	texture = GameEngine::Instance().GetDefaultTexture();
}

void Texture::Copy(Texture& other)
{}

std::shared_ptr<SDL_Texture> Texture::GetTexture() const
{
	return texture;
}

Vec2<float> Texture::GetSize() const
{
	return Vec2<float>(texture->w,texture->h);
}

void Texture::NativeSetRenderLayer(LayerHierarchy new_layer)
{
	layer_hierarchy = new_layer;
}

void Texture::SetTexture(const std::shared_ptr<SDL_Texture> &new_texture)
{
	if (new_texture)
	{
		w = new_texture->w;
		h = new_texture->h;
		texture = new_texture;
	}
	else
	{
		w = 0;
		h = 0;
	}
}

