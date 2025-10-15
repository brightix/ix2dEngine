#include <SDL3_ttf/SDL_ttf.h>

#include "../System/Font.hpp"
#include "Utilities/Exception.hpp"
#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
#include "../Classes/Core/GameEngine.hpp"
#include "Classes/Component/SenceComponent/StaticTexture.hpp"

void FontRenderer::Init()
{
    if (!TTF_Init()) {
        Log("TTF 初始化失败");
        throw std::runtime_error("TTF_Init failed: " + std::string(SDL_GetError()));
        return;
    }
    renderer = GameEngine::Instance().GetRenderer();
    fontCache.onEvict = [](TTF_Font*& font){
        if(font) {
            TTF_CloseFont(font); // �ͷ�������Դ
            font = nullptr;      // ��������ָ��
        }
    };

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
	name = "FontRenderer";
}

FontRenderer::FontRenderer() : fontCache(3)
{
    Init();
}

FontRenderer& FontRenderer::Instance()
{
    static FontRenderer instance;
    return instance;
}

TTF_Font* FontRenderer::GetFont(std::string fontName,size_t size)
{
    std::string request = fontName + std::to_string(size);
    auto font = fontCache.get(request);
    if(!font)
    {
        if(!LoadFont(fontName,size))
        {
            Log("字体加载失败，已返回默认simkai字体");
            LoadFont("simkai",size);
            return *fontCache.get("simkai");
        }
        return *fontCache.get(request);
    }
    return *font;
}

bool FontRenderer::LoadFont(std::string fontName,size_t size)
{
    //fontMap[fontName].c_str();
    if(TTF_Font* font = TTF_OpenFont(fontMap[fontName].c_str(), size))
    {
        fontCache.put(fontName + std::to_string(size),font);
        return true;
    }
    //throw std::runtime_error("Failed to load font: " + fontName + "\nSDL_ttf Error: " + SDL_GetError());
    return false;
}

StaticTexture* FontRenderer::GetTextTexture(std::string str, std::string fontName, size_t fontSize, SDL_Color col)
{
    //
    try{
        TTF_Font* font = GetFont(fontName,fontSize);
        SDL_Surface* surface = TTF_RenderText_Blended(font, str.c_str(), str.length(), col);
        SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND); // 启用混合模式
        StaticTexture* texture = new StaticTexture(SDL_CreateTextureFromSurface(renderer, surface));
        SDL_DestroySurface(surface); // 清除CPU缓存
        return texture;
    }catch(const Exception& e){
        Log(e.what());
    }
    return {};
}

void FontRenderer::UpdateTextTexture(SDL_Texture *texture,
                                     const std::string &str,
                                     const std::string &fontName,
                                     size_t fontSize,
                                     const SDL_Color col)
{
    try {
        TTF_Font* font = GetFont(fontName, fontSize);
        if (!font) {
            Log("GetFont failed");
            return;
        }
        SDL_Surface* surface = TTF_RenderText_Blended(font, str.c_str(), str.length(), SDL_Color{100,0,100,150});
        //SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND); // 启用混合模式
        // auto* temptex = SDL_CreateTextureFromSurface(renderer,surface);
        // auto dstt = SDL_FRect(400,700,temptex->w,temptex->h);
        // SDL_RenderTexture(renderer,temptex,nullptr,&dstt);
        // SDL_DestroyTexture(temptex);

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

    // SDL_SetRenderTarget(renderer,texture->GetTexture());
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    // SDL_RenderClear(renderer);
    // SDL_RenderTexture(renderer, some_other_texture, nullptr, &dstRect);
    // SDL_SetRenderTarget(renderer, nullptr);
    //
    // void* pixels;
    // int pitch;
    // SDL_LockTexture(texture->GetTexture(), nullptr, &pixels, &pitch);
    // memcpy(pixels, surface->pixels, surface->h * pitch);
    // SDL_UnlockTexture(texture->GetTexture());
}

FontRenderer::~FontRenderer()
{
    TTF_Quit();
}