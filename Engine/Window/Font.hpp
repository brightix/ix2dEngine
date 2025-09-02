#pragma once
#include <SDL3_ttf/SDL_ttf.h>
#include <FuncLib/ixStaticFuncLib.hpp>
#include <DataStructure/LRUCache.hpp>
class FontRenderer
{
    LRUCache<std::string,TTF_Font*> fontMap;
public:
    FontRenderer() : fontMap(3)
    {
        fontMap.onEvict = [](TTF_Font*& font){
            if(font) {
                TTF_CloseFont(font); // 释放字体资源
                font = nullptr;      // 避免悬空指针
            }
        };  
        TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/Arial.ttf", 24); // 字体路径 + 字号
        if (!font) {
            LogToFile("字体文件加载失败",Error);
            return;
        }
    }
    ~FontRenderer()
    {
        TTF_Quit();
    }

};