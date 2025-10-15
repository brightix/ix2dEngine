#pragma once
#include <memory>

//#define sptr std::shared_ptr
#define UpDir    Vec2<int>(0, -1)
#define DownDir  Vec2<int>(0, 1)
#define LeftDir  Vec2<int>(-1, 0)
#define RightDir Vec2<int>(1, 0)


#define DebugMod 1


#define NAME(x) x + std::string("_") + std::to_string(glo_id); glo_id++

#define TEventParams std::optional<EventParams>

// #define vector





#define Color_White SDL_Color(255,255,255,255)

#define Color_Red SDL_Color(255,0,0,255)



//#define AddComponent(name) AddActorComponent<x>(name, x)