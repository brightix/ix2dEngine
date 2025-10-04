#include "System/Texture.hpp"

#include "System/GameEngine.hpp"

StaticTexture::StaticTexture() : texture(nullptr), w(0), h(0) {
}


StaticTexture::StaticTexture(const Vec2d<float> size, const SDL_Color color, bool is_fill)
{
	w = size.x;
	h = size.y;
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

	int p = pitch / 4;

	int sx = size.x;
	int sy = size.y;
	if (is_fill)
	{
		for (int y = 0; y < size.x; ++y)
		{
			for (int x = 0; x < size.y; ++x)
			{
				dst[y * p + x] = pixelColor;
			}
		}
	}
	else
	{
		const int out_line_width = 3;
		for (int y = 0; y < sx; ++y)
		{
			for (int x = 0; x < sy; ++x)
			{
				// 判断是否属于上边缘描边（顶部向内扩展out_line_width像素）
				bool is_top_edge = (y < out_line_width);
				// 判断是否属于下边缘描边（底部向内扩展out_line_width像素）
				bool is_bottom_edge = (y >= sx - out_line_width);
				// 判断是否属于左边缘描边（左侧向内扩展out_line_width像素，且不在上下边缘覆盖区）
				bool is_left_edge = (x < out_line_width) && (y >= out_line_width) && (y < sx - out_line_width);
				// 判断是否属于右边缘描边（右侧向内扩展out_line_width像素，且不在上下边缘覆盖区）
				bool is_right_edge = (x >= sy - out_line_width) && (y >= out_line_width) && (y < sx - out_line_width);

				// 只要属于任意一条边的描边范围，就绘制像素
				if (is_top_edge || is_bottom_edge || is_left_edge || is_right_edge)
				{
					dst[y * p + x] = pixelColor;
				}
			}
		}
	}
	SDL_UnlockTexture(texture);
}

StaticTexture::StaticTexture(SDL_Texture *texture) : texture(texture), w(texture->w), h(texture->h) {}

StaticTexture::~StaticTexture()
{
	SDL_DestroyTexture(texture);
}

SDL_Texture * StaticTexture::GetTexture() const
{
	return texture;
}
