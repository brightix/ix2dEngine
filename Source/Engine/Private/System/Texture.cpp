#include "../../Structure/Texture.hpp"

#include "../../Classes/Core/GameEngine.hpp"

StaticTexture::StaticTexture() : texture(nullptr), w(0), h(0)
{

}


StaticTexture::StaticTexture(const Vec2<int> size, const SDL_Color color, const bool is_fill)
{
	w = size.x;
	h = size.y;

	texture = SDL_CreateTexture(
		GameEngine::Instance().GetRenderer(),
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		w,
		h
	);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	auto r = GameEngine::Instance().GetRenderer();
	SDL_SetRenderTarget(r, texture);

	if (is_fill)
	{
		const auto rect = SDL_FRect(0,0,w,h);
		SDL_RenderFillRect(r,&rect);
	}
	else
	{
		// 清空背景
		SDL_SetRenderDrawColor(r, 0, 0, 0, 0); // 透明背景
		SDL_RenderClear(r);
		SDL_SetRenderDrawColor(r, 0, 255, 255, 255); // 青色边框
		int thickness = 3;
		for (int i = 0; i < thickness; ++i) {
			SDL_FRect rect = SDL_FRect(i,i, w - i * 2, h - i * 2);
			SDL_RenderRect(r, &rect);
		}
	}

	SDL_SetRenderTarget(r, nullptr);
// 	if (!texture)
// 	{
// 		SDL_Log("Failed to create texture: %s", SDL_GetError());
// 		LogWithLevel(std::string("Failed to create texture") + SDL_GetError(),LogLevel::FatalError);
// 	}
//
// 	// 锁定纹理获取像素指针
// 	void* pixels;
// 	int pitch;
// 	if (!SDL_LockTexture(texture, nullptr, &pixels, &pitch))
// 	{
// 		SDL_DestroyTexture(texture);
// 		SDL_Log("%s",SDL_GetError());
// 		LogWithLevel(std::string("Failed to lock texture") + SDL_GetError(),LogLevel::FatalError);
// 	}
//
// 	// 填充颜色 (pitch 表示每行的字节数)
//
// 	auto* dst = static_cast<Uint32*>(pixels);
// 	Uint32 pixelColor = (color.a << 24) | (color.b << 16) | (color.g << 8) | (color.r);
//
// 	int p = pitch / 4;
//
// 	if (is_fill)
// 	{
// 		for (int y = 0; y < h; ++y)
// 		{
// 			for (int x = 0; x < w; ++x)
// 			{
// 				dst[y * p + x] = pixelColor;
// 			}
// 		}
// 	}
// 	else
// 	{
// 		for (int y = 0; y < h; ++y)
// 		{
// 			for (int x = 0; x < w; ++x)
// 			{
// 				constexpr int out_line_width = 3;
// 				// 判断是否属于上边缘描边（顶部向内扩展out_line_width像素）
// 				bool is_top_edge = (y < out_line_width);
// 				// 判断是否属于下边缘描边（底部向内扩展out_line_width像素）
// 				bool is_bottom_edge = (y >= w - out_line_width);
// 				// 判断是否属于左边缘描边（左侧向内扩展out_line_width像素，且不在上下边缘覆盖区）
// 				bool is_left_edge = (x < out_line_width) && (y >= out_line_width) && (y < w - out_line_width);
// 				// 判断是否属于右边缘描边（右侧向内扩展out_line_width像素，且不在上下边缘覆盖区）
// 				bool is_right_edge = (x >= h - out_line_width) && (y >= out_line_width) && (y < w - out_line_width);
//
// 				// 只要属于任意一条边的描边范围，就绘制像素
// 				if (is_top_edge || is_bottom_edge || is_left_edge || is_right_edge)
// 				{
// 					dst[y * p + x] = pixelColor;
// 				}
// 			}
// 		}
// 	}
// 	SDL_UnlockTexture(texture);
}

StaticTexture::StaticTexture(SDL_Texture* texture, SDL_TextureAccess texture_mode) : texture(texture), w(texture->w), h(texture->h) {}
StaticTexture::StaticTexture(StaticTexture& other) {
	texture = other.texture;
	w = other.w;
	h = other.h;
}
StaticTexture::StaticTexture(StaticTexture&& other)
 noexcept {
	texture = other.texture;
	w = other.w;
	h = other.h;
	other.texture = nullptr;
}

void StaticTexture::SetPivot(Vec2<float> p)
{
	pivot = p;
}

StaticTexture::~StaticTexture()
{
	if (texture) SDL_DestroyTexture(texture);
}

void StaticTexture::Copy(StaticTexture& other)
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
	SDL_RenderTexture(renderer, other.texture, nullptr, nullptr);

	// 恢复原渲染目标
	SDL_SetRenderTarget(renderer, origTarget);

}

SDL_Texture* StaticTexture::GetTexture() const
{
	return texture;
}
