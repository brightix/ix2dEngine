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
    // LRUCache<std::string,std::shared_ptr<TTF_Font>> fontCache;
	std::unordered_map<std::string,std::string> fontMap;
	std::unordered_map<std::string,std::shared_ptr<TTF_Font>> fonts;
    FontRenderer();
public:
    static FontRenderer& Instance();
    //获取字体
    std::shared_ptr<TTF_Font> GetFont(const std::string &fontName = "simkai",size_t size = 24);

    //无缓存 加载字体
    bool LoadFont(const std::string &fontName,size_t size);



    // [[nodiscard]] StaticTexture* GetTextTexture(std::string str, std::string fontName = "simkai", size_t fontSize = 24,
    //                                             SDL_Color col = {255, 255, 255, 255});

    void UpdateTextTexture(SDL_Texture *texture, const std::string &str, const FontStyle &style = {});
	const std::unordered_map<std::string,std::shared_ptr<TTF_Font>>* GetFontMap() const ;
    ~FontRenderer();
};
std::shared_ptr<TTF_Font> GetFont(const std::string &fontName, size_t size);
std::shared_ptr<SDL_Surface> GetTextSurface(const std::string& str, const FontStyle &fs = FontStyle());