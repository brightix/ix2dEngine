#include <SDL3_ttf/SDL_ttf.h>

#include "../System/Font.hpp"
#include "Utilities/Exception.hpp"
#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
#include "../System/GameEngine.hpp"

void FontRenderer::Init()
{
    if (!TTF_Init()) {
        Log("TTF��ʼ��ʧ��");
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

    //������������
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
        try{
            if(!LoadFont(fontName,size))//����û�и�����
            {
                throw Exception("δ�ҵ�����");
            }
            return *fontCache.get(request);
        }catch (const std::exception& e) {
            std::cerr << "�쳣����: " << e.what() << std::endl;
            return nullptr;
        }
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
    throw std::runtime_error("Failed to load font: " + fontName + "\nSDL_ttf Error: " + SDL_GetError());
    return false;
}

GCPtr<StaticTexture> FontRenderer::GetTextTexture(std::string str,std::string fontName, size_t fontSize, SDL_Color col)
{
    //
    try{
        TTF_Font* font = GetFont(fontName,fontSize);
        SDL_Surface* surface = TTF_RenderText_Blended(font, str.c_str(), str.length(), col);
        GCPtr<StaticTexture> texture = ConstructObjectFromClass<StaticTexture>(new StaticTexture(SDL_CreateTextureFromSurface(renderer, surface)));
        SDL_DestroySurface(surface); // 清除CPU缓存
        return texture;
    }catch(const Exception& e){
        Log(e.what());
        //std::cout << e.what() << endl;
    }
    return {};
}
void FontRenderer::UpdateTextTexture(const StaticTexture *texture,
                                     const std::string &str,
                                     const std::string &fontName,
                                     size_t fontSize,
                                     SDL_Color col)
{
    try {
        TTF_Font* font = GetFont(fontName, fontSize);

        // 1. 渲染文字到 surface
        SDL_Surface* surface = TTF_RenderText_Blended(font, str.c_str(), str.length(), col);

        // 2. 获取纹理大小
        int w = surface->w;
        int h = surface->h;

        // 3. Lock texture
        void* pixels;
        int pitch;
        SDL_LockTexture(texture->GetTexture(), nullptr, &pixels, &pitch);

        // 4. 拷贝 surface 内容到 texture 内存
        // 注意 surface->pitch 可能和 texture pitch 不同
        for (int y = 0; y < h; y++) {
            memcpy(static_cast<Uint8 *>(pixels) + y * pitch,
                   static_cast<Uint8 *>(surface->pixels) + y * surface->pitch,
                   w * 4); // 假设 RGBA8888
        }

        SDL_UnlockTexture(texture->GetTexture());
        SDL_DestroySurface(surface); // 清除 CPU 缓存

    } catch (const Exception& e) {
        Log(e.what());
    }
}
FontRenderer::~FontRenderer()
{
    TTF_Quit();
}