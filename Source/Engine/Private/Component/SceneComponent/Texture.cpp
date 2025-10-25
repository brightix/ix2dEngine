#include "Classes/Component/SenceComponent/Texture.hpp"

#include "Classes/Core/GameEngine.hpp"
#include "Utilities/FuncLib/Deleter.hpp"


Texture::Texture()
{
	//先引用默认渲染图
	NAME;
	in_texture = GetDefaultTexture();
	name = "Texture";
	w = in_texture->w;
	h = in_texture->h;
	// if (texture_)
	// {
	// 	SetStaticTexture(texture_);
	// }
}

void Texture::Construct()
{
	SceneComponent::Construct();
	//in_texture = GameEngine::Instance().renderer_center->DefaultTexture;
}

void Texture::SetPivot(Vec2<float> p)
{
	pivot = p;
}


void Texture::Copy(Texture& other)
{}

std::shared_ptr<SDL_Texture> Texture::GetTexture() const
{
	return in_texture;
}

Vec2<float> Texture::GetSize() const
{
	return Vec2<float>(w,h);
}

TextureType Texture::GetTextureType()
{
	return TextureType::StaticTexture;
}

void Texture::SafeDestroyTexture(SDL_Texture* texture)
{
	if (texture->refcount == 1)
	{
		SDL_DestroyTexture(texture);
	}
}

void Texture::SetNewTexture(std::shared_ptr<SDL_Texture> new_texture)
{
	in_texture = new_texture;
	if (new_texture)
	{
		w = new_texture->w;
		h = new_texture->h;
	}
	else
	{
		w = 0;
		h = 0;
	}
}


// void Texture::AsyncSetTextureFromSurface(std::shared_ptr<SDL_Surface> new_surface)
// {
// 	auto temp = TTexture(nullptr);
//
// 	RenderTask task;
// 	task.task = [new_surface, temp](SDL_Renderer* r) mutable{
// 		temp.reset(SDL_CreateTextureFromSurface(r, new_surface.get()),SDLTextureDeleter());
// 	};
// 	task.callback = [this, temp]() {
//
// 		SetStaticTexture(temp);
// 	};
// 	NewRendererTask(task);
// }
//
// void Texture::AsyncLoadOutLine()
// {
// 	auto task = RenderTask();
// 	auto tex = TTexture(nullptr);
// 	task.task = [this,&tex](SDL_Renderer* r) {
// 		if (!w || !h)
// 		{
// 			w = 500;
// 			h = 500;
// 		}
// 		tex = RendererCenter::CreateOutLineTexture({0,0,w,h});
// 	};
// 	task.callback = [this,tex]() {
// 		SetStaticTexture(tex);
// 	};
// }

