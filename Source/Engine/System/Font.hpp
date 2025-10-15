#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <filesystem>
#include <unordered_map>

#include "../Classes/Component/SenceComponent/Texture.hpp"
#include "Classes/Object.hpp"
#include "Utilities/Exception.hpp"
#include "Utilities/DataStructure/LRUCache.hpp"

class FontRenderer : public Object
{
    SDL_Renderer* renderer;
    LRUCache<std::string,TTF_Font*> fontCache;
    std::unordered_map<std::string,std::string> fontMap;
    SDL_Color color;
    TTF_Font* default_font;
    FontRenderer();
public:
    //FontRenderer(size_t size);
    static FontRenderer& Instance();
    //获取字体
    TTF_Font* GetFont(std::string fontName,size_t size);

    //无缓存 加载字体
    bool LoadFont(std::string fontName,size_t size);

    StaticTexture *GetTextTexture(std::string str, std::string fontName = "simkai", size_t fontSize = 24,
                                  SDL_Color col = {255, 255, 255, 255});

    void UpdateTextTexture(SDL_Texture *texture, const std::string &str, const std::string &fontName = "simkai", size_t fontSize = 24, SDL_Color
                           col = {0, 0, 255, 255});

    ~FontRenderer() override;
private:
    void Init();
};