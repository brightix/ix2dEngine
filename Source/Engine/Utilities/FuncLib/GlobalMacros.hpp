#pragma once
#include <typeinfo>
#define UpDir    Vec2<int>(0, -1)
#define DownDir  Vec2<int>(0, 1)
#define LeftDir  Vec2<int>(-1, 0)
#define RightDir Vec2<int>(1, 0)
#define PI 3.14159265

#define DEBUG 1
#define TRACINGTEST 0

//#define NAME(x) x + std::string("_") + std::to_string(glo_id); glo_id++
#define CNAME class_name = typeid(*this).name(); \
					name = class_name + std::to_string(id)

#define TEventParams std::optional<EventParams>

#define Cast_(T,U) dynamic_cast<T>(U)

#define NewRendererTask(T) RendererCenterBackUp::AddRendererTask(T)

#define TTexture(x)     std::shared_ptr<SDL_Texture>(x,SDLTextureDeleter())
#define TSurface(x)	 std::shared_ptr<SDL_Surface>(x,SDLSurfaceDeleter())
#define TFont(x)		 std::shared_ptr<TTF_Font>(x,SDLTTFDeleter())


#define RenderCallback_SetStaticTexture(Texture,tex) [&Texture,tex] {	\
	texture->SetStaticTexture(tex.get());								\
}																		\

#define WHITE SDL_Color(255,255,255,255)
#define RED SDL_Color(255,0,0,255)
#define GREEN SDL_Color(0,255,0,255)
#define BLUE SDL_Color(0,0,255,255)
#define EmeraldGreen SDL_Color(80,200,120,255)
#define YELLOW SDL_Color(255,255,0,255)

#if TRACINGTEST == 1
	#define TStart TracingUtility::Instance().StartTracing(__FUNCTION__)
	#define TEnd TracingUtility::Instance().EndTracing(__FUNCTION__)
	#define TStartF(file) TracingUtility::Instance().StartTracing(file)
	#define TEndF(file) TracingUtility::Instance().EndTracing(file)
#else
	#define TStart
	#define TEnd
	#define TStartF(file)
	#define TEndF(file)
#endif

#if DEBUG == 1
	#define BREAK std::cout << "break point tool" << std::endl;
#else
	#define BREAK
#endif

#define STOP_IF(x) if(x) STOP();

#define PIVOT_BOTTOM_CENTER Vec2<float>(0.5f, 1.f)



#define TypeID(T) std::type_index(typeid(T))