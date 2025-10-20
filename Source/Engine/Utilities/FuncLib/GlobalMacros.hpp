#pragma once
#include <typeinfo>
//#define sptr std::shared_ptr
#define UpDir    Vec2<int>(0, -1)
#define DownDir  Vec2<int>(0, 1)
#define LeftDir  Vec2<int>(-1, 0)
#define RightDir Vec2<int>(1, 0)


#define DebugMod 1


//#define NAME(x) x + std::string("_") + std::to_string(glo_id); glo_id++
#define NAME name = typeid(*this).name()

#define TEventParams std::optional<EventParams>

#define Cast_(T,U) dynamic_cast<T>(U)

#define NewRendererTask(T) RendererCenter::AddRendererTask(T)

#define TTexture(x)     std::shared_ptr<SDL_Texture>(x,SDLTextureDeleter())
#define TSurface	 std::shared_ptr<SDL_Surface>(nullptr,SDLSurfaceDeleter())
#define TFont		 std::shared_ptr<TTF_Font>(nullptr,SDLTTFDeleter())


#define RenderCallback_SetStaticTexture(Texture,tex) [&Texture,tex] {	\
	texture->SetStaticTexture(tex.get());								\
}																		\

#define Color_White SDL_Color(255,255,255,255)

#define Color_Red SDL_Color(255,0,0,255)



//#define AddComponent(name) AddActorComponent<x>(name, x)