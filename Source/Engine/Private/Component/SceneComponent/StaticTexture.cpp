#include "Classes/Component/SenceComponent/StaticTexture.hpp"

#include "Classes/Core/GameEngine.hpp"


StaticTexture::StaticTexture()
{
	component_name = NAME("StaticTexture");
}


StaticTexture::StaticTexture(const StaticTexture& other)
{
	//SetStaticTexture(other.in_texture.get());
	in_texture = other.in_texture;
	w = other.w;
	h = other.h;
	pivot = other.pivot;
}


TextureType StaticTexture::GetTextureType()
{
	return TextureType::StaticTexture;
}

void StaticTexture::ComponentRender()
{
	SDL_Renderer* renderer = GameEngine::Instance().GetRenderer();
	const SDL_FRect dst(transform.location.x,transform.location.y, h, w);
	SDL_RenderTexture(renderer,in_texture.get(),nullptr,&dst);
}

void StaticTexture::LoadDefaultTexture(const Vec2<int> size, const SDL_Color color, const bool is_fill)
{
	w = size.x;
	h = size.y;

	auto texture = SDL_CreateTexture(
		GameEngine::Instance().GetRenderer(),
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		w,
		h
	);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);


	if (is_fill)
	{
		CreateFilledTexture({0,0,w,h});
	}
	else
	{
		CreateOutLineTexture({0,0,w,h});
	}
	SetStaticTexture(texture);
}
// StaticTexture::StaticTexture(StaticTexture&& other)
//  noexcept {
// 	texture = other.texture;
// 	w = other.w;
// 	h = other.h;
// 	other.texture = nullptr;
// }

StaticTexture::~StaticTexture()
{
	//防止同时多个实例引用这个纹理
	// if (in_texture)
	// {
	// 	SDL_DestroyTexture(in_texture);
	// }
}



// StaticTexture::StaticTexture(const Vec2<int> size, const SDL_Color color, const bool is_fill)
// {
//
// 	// 	if (!texture)
// 	// 	{
// 	// 		SDL_Log("Failed to create texture: %s", SDL_GetError());
// 	// 		LogWithLevel(std::string("Failed to create texture") + SDL_GetError(),LogLevel::FatalError);
// 	// 	}
// 	//
// 	// 	// 锁定纹理获取像素指针
// 	// 	void* pixels;
// 	// 	int pitch;
// 	// 	if (!SDL_LockTexture(texture, nullptr, &pixels, &pitch))
// 	// 	{
// 	// 		SDL_DestroyTexture(texture);
// 	// 		SDL_Log("%s",SDL_GetError());
// 	// 		LogWithLevel(std::string("Failed to lock texture") + SDL_GetError(),LogLevel::FatalError);
// 	// 	}
// 	//
// 	// 	// 填充颜色 (pitch 表示每行的字节数)
// 	//
// 	// 	auto* dst = static_cast<Uint32*>(pixels);
// 	// 	Uint32 pixelColor = (color.a << 24) | (color.b << 16) | (color.g << 8) | (color.r);
// 	//
// 	// 	int p = pitch / 4;
// 	//
// 	// 	if (is_fill)
// 	// 	{
// 	// 		for (int y = 0; y < h; ++y)
// 	// 		{
// 	// 			for (int x = 0; x < w; ++x)
// 	// 			{
// 	// 				dst[y * p + x] = pixelColor;
// 	// 			}
// 	// 		}
// 	// 	}
// 	// 	else
// 	// 	{
// 	// 		for (int y = 0; y < h; ++y)
// 	// 		{
// 	// 			for (int x = 0; x < w; ++x)
// 	// 			{
// 	// 				constexpr int out_line_width = 3;
// 	// 				// 判断是否属于上边缘描边（顶部向内扩展out_line_width像素）
// 	// 				bool is_top_edge = (y < out_line_width);
// 	// 				// 判断是否属于下边缘描边（底部向内扩展out_line_width像素）
// 	// 				bool is_bottom_edge = (y >= w - out_line_width);
// 	// 				// 判断是否属于左边缘描边（左侧向内扩展out_line_width像素，且不在上下边缘覆盖区）
// 	// 				bool is_left_edge = (x < out_line_width) && (y >= out_line_width) && (y < w - out_line_width);
// 	// 				// 判断是否属于右边缘描边（右侧向内扩展out_line_width像素，且不在上下边缘覆盖区）
// 	// 				bool is_right_edge = (x >= h - out_line_width) && (y >= out_line_width) && (y < w - out_line_width);
// 	//
// 	// 				// 只要属于任意一条边的描边范围，就绘制像素
// 	// 				if (is_top_edge || is_bottom_edge || is_left_edge || is_right_edge)
// 	// 				{
// 	// 					dst[y * p + x] = pixelColor;
// 	// 				}
// 	// 			}
// 	// 		}
// 	// 	}
// 	// 	SDL_UnlockTexture(texture);
// }