#pragma once
#include <memory>

//#define sptr std::shared_ptr
#define UpDir    Vec2d<int>(0, -1)
#define DownDir  Vec2d<int>(0, 1)
#define LeftDir  Vec2d<int>(-1, 0)
#define RightDir Vec2d<int>(1, 0)


#define DebugMod 1


#define TEventParams std::optional<EventParams>

#define Color_White SDL_Color(255,255,255,255)

#define Color_Red SDL_Color(255,0,0,255)