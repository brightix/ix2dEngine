#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <filesystem>
#include <unordered_map>
#include "Utilities/Exception.hpp"
#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
#include "Utilities/DataStructure/LRUCache.hpp"

class FontRenderer
{
    using Texture = std::shared_ptr<SDL_Texture>;
    SDL_Renderer* renderer;
    LRUCache<std::string,TTF_Font*> fontCache;
    std::unordered_map<std::string,std::string> fontMap;
    SDL_Color color;
    FontRenderer();
public:
    //FontRenderer(size_t size);
    static FontRenderer& Instance();
    //获取字体
    TTF_Font* GetFont(std::string fontName,size_t size);

    //无缓存 加载字体
    bool LoadFont(std::string fontName,size_t size);
    Texture GetTextTexture(std::string str,std::string fontName = "simkai", size_t fontSize = 24, SDL_Color col = {255,255,255,255});
    ~FontRenderer();
private:
    void Init();
};