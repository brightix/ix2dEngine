#include "Classes/Component/SenceComponent/Texture.hpp"

#include "Classes/Core/GameEngine.hpp"
#include "Types/FRect.hpp"


Texture::Texture(SDL_Texture* texture_, SDL_TextureAccess texture_mode)
{
	if (texture_)
	{
		SetStaticTexture(texture_);
	}
}

void Texture::SetPivot(Vec2<float> p)
{
	pivot = p;
}


void Texture::Copy(Texture& other)
{
	w = other.w;
	h = other.h;
	auto renderer = GameEngine::Instance().GetRenderer();
	SDL_Texture* newTexture = SDL_CreateTexture(renderer,
	SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
	w, h);
	// 保存原渲染目标
	SDL_Texture* origTarget = SDL_GetRenderTarget(renderer);

	// 设置新纹理为渲染目标
	SDL_SetRenderTarget(renderer, newTexture);

	// 渲染原纹理到新纹理
	SDL_RenderTexture(renderer, other.in_texture.get(), nullptr, nullptr);

	// 恢复原渲染目标
	SDL_SetRenderTarget(renderer, origTarget);

}

SDL_Texture* Texture::GetTexture() const
{
	return in_texture.get();
}

Vec2<float> Texture::GetSize() const
{
	return Vec2<float>(w,h);
}

void Texture::SafeDestroyTexture(SDL_Texture* texture)
{
	if (texture->refcount == 1)
	{
		SDL_DestroyTexture(texture);
	}
}

void Texture::SetStaticTexture(SDL_Texture* new_texture)
{
	in_texture = std::shared_ptr<SDL_Texture>(new_texture,SDLTextureDeleter());
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

SDL_Texture * Texture::CreateOutLineTexture(const FRect& rect)
{
	SDL_Texture* texture_T = SDL_CreateTexture(
		GameEngine::Instance().GetRenderer(),
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		rect.w,
		rect.h
	);
	auto r = GameEngine::Instance().GetRenderer();
	SDL_SetRenderTarget(r, texture_T);
	// 清空背景
	SDL_SetRenderDrawColor(r, 0, 0, 0, 0); // 透明背景
	SDL_RenderClear(r);
	SDL_SetRenderDrawColor(r, 0, 255, 255, 255); // 青色边框
	int thickness = 3;
	for (int i = 0; i < thickness; ++i) {
		SDL_FRect rect_bound = SDL_FRect(i,i, rect.w - i * 2, rect.h - i * 2);
		SDL_RenderRect(r, &rect_bound);
	}
	SDL_SetRenderTarget(r,nullptr);
	return texture_T;
}
SDL_Texture * Texture::CreateFilledTexture(const FRect& rect)
{
	auto r = GameEngine::Instance().GetRenderer();
	SDL_Texture* texture_T = SDL_CreateTexture(
		GameEngine::Instance().GetRenderer(),
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		rect.w,
		rect.h
	);
	SDL_SetRenderTarget(r, texture_T);
	const auto rect_T = SDL_FRect(0, 0, rect.w, rect.h);
	SDL_RenderFillRect(r,&rect_T);
	SDL_SetRenderTarget(r, nullptr);
	return texture_T;
}
