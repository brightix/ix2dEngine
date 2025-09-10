#include "System/Texture.hpp"

#include "System/GameEngine.hpp"

Texture::Texture() : texture(nullptr) {}

Texture::Texture(Vec2d<float> size, SDL_Color color)
{
	texture = SDL_CreateTexture(
		GameEngine::Instance().GetRenderer(),
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STREAMING,
		size.x,
		size.y
	);

	if (!texture)
	{
		SDL_Log("Failed to create texture: %s", SDL_GetError());
		LogWithLevel(std::string("Failed to create texture") + SDL_GetError(),LogLevel::FatalError);
	}

	// 锁定纹理获取像素指针
	void* pixels;
	int pitch;
	if (!SDL_LockTexture(texture, nullptr, &pixels, &pitch))
	{
		SDL_DestroyTexture(texture);
		SDL_Log("%s",SDL_GetError());
		LogWithLevel(std::string("Failed to lock texture") + SDL_GetError(),LogLevel::FatalError);
	}

	// 填充颜色 (pitch 表示每行的字节数)
	Uint32* dst = static_cast<Uint32*>(pixels);
	Uint32 pixelColor = (color.a << 24) | (color.b << 16) | (color.g << 8) | (color.r);

	for (int y = 0; y < size.x; ++y)
	{
		for (int x = 0; x < size.y; ++x)
		{
			dst[y * (pitch / 4) + x] = pixelColor;
		}
	}

	SDL_UnlockTexture(texture);
}
