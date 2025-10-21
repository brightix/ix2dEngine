#include "../System/Font.hpp"

#include "Utilities/Exception.hpp"
#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
#include "../Classes/Core/GameEngine.hpp"
#include "Types/FontStyle.hpp"
#include "Utilities/FuncLib/Deleter.hpp"


FontRenderer::FontRenderer() : fontCache(3)
{
	if (!TTF_Init()) {
		Log("TTF 初始化失败");
		throw std::runtime_error("TTF_Init failed: " + std::string(SDL_GetError()));
		return;
	}
	// fontCache.OnEvict = [](TTF_Font*& font){
	//     if(font) {
	//         TTF_CloseFont(font);
	//         font = nullptr;
	//     }
	// };

	std::string path = "Source/Engine/System/Fonts/";
	for(const auto& entry : std::filesystem::directory_iterator(path))
	{
		if(entry.is_regular_file())
		{
			std::string ext = entry.path().extension().string();
			if(ext == ".ttf")
			{
				std::string name = entry.path().filename().stem().string();
				std::string total = entry.path().string();
				std::cout << total << std::endl;
				fontMap[name] = entry.path().string();
			}
		}
	}
	default_font = TTF_OpenFont(path.c_str(), 64);
	//GCAllObjects.push_back(this);
	NAME;
}

FontRenderer& FontRenderer::Instance()
{
    static FontRenderer instance;
    return instance;
}

// FontStyle FontRenderer::GetFontStyle(std::string fontName,size_t size)
// {
//     std::string request = fontName + std::to_string(size);
//     auto font = fontCache.get(request);
//     if(!font)
//     {
//         if(!LoadFont(fontName,size))
//         {
//             Log("字体加载失败，已返回默认simkai字体");
//             LoadFont("simkai",size);
//             return *fontCache.get("simkai");
//         }
//         return *fontCache.get(request);
//     }
//     return *font;
// }

bool FontRenderer::LoadFont(std::string fontName,size_t size)
{
    //fontMap[fontName].c_str();
	auto font = std::shared_ptr<TTF_Font>(TTF_OpenFont(fontMap[fontName].c_str(), size),SDLTTFDeleter());
    if(font.get())
    {
        fontCache.put(fontName + std::to_string(size),font);
        return true;
    }
    return false;
}

std::shared_ptr<SDL_Surface> FontRenderer::GetTextSurface(const std::string& str, FontStyle fs)
{
	auto surface = TSurface;
	surface.reset(TTF_RenderText_Blended(fs.font.get(), str.c_str(), str.length(), fs.text_color));
	SDL_SetSurfaceBlendMode(surface.get(), SDL_BLENDMODE_BLEND); // 启用混合模式
	return surface;
}


//可以加入多线程豪华套餐
void FontRenderer::UpdateTextTexture(SDL_Texture *texture,
                                     const std::string &str,
                                     FontStyle style)
{
    try {
        SDL_Surface* surface = TTF_RenderText_Blended(style.font.get(), str.c_str(), str.length(), SDL_Color{100,0,100,150});
        if (!surface)
        {
            //Log("TTF_RenderText_Blended failed: " + std::string(TTF_GetError()));
            return;
        }

        if (!texture)
        {
            Log("Texture is null");
            SDL_DestroySurface(surface);
            return;
        }
        // SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        auto dst = SDL_Rect{0,0,surface->w,surface->h};
        SDL_UpdateTexture(texture, &dst, surface->pixels, surface->pitch);

        SDL_DestroySurface(surface);

    } catch (const Exception& e) {
        Log(e.what());
    }
}

FontRenderer::~FontRenderer()
{
    TTF_Quit();
}