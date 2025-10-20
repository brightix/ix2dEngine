#pragma once

#include <filesystem>
#include <unordered_map>
#include <SDL3_ttf/SDL_ttf.h>

#include "../Classes/Component/SenceComponent/Texture.hpp"
#include "Utilities/Exception.hpp"
#include "Utilities/DataStructure/LRUCache.hpp"
#include "Types/FontStyle.hpp"
class FontRenderer// : public Object
{
	std::string name;
    LRUCache<std::string,std::shared_ptr<TTF_Font>> fontCache;
    std::unordered_map<std::string,std::string> fontMap;
    TTF_Font* default_font;
    FontRenderer();
public:
    static FontRenderer& Instance();
    //获取字体
    TTF_Font* GetFont(std::string fontName,size_t size);

    //无缓存 加载字体
    bool LoadFont(std::string fontName,size_t size);

    static std::shared_ptr<SDL_Surface> GetTextSurface(const std::string& str, FontStyle fs = {});

    // [[nodiscard]] StaticTexture* GetTextTexture(std::string str, std::string fontName = "simkai", size_t fontSize = 24,
    //                                             SDL_Color col = {255, 255, 255, 255});

    void UpdateTextTexture(SDL_Texture *texture, const std::string &str, FontStyle style = {});

    ~FontRenderer();
};
