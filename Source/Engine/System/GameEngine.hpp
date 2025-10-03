#pragma once

#include <SDL3/SDL.h>
#include "GameWorld.hpp"
#include "Texture.hpp"
#include "Classes/Core/TimerSystem.hpp"
#include "Utilities/Timer.hpp"
#include "Structure/SystemConfig.hpp"
#include "Utilities/ConverterRegistry.hpp"
#include "Utilities/ExternalWrapper.hpp"
#include "Utilities/GCPtr.hpp"

class GameEngine : public Object
{
    SDL_Renderer* renderer;
    SDL_Window* window;

    bool running = true;
    //系统数据
    SystemConfig SysConfig{};

    double delta_time;
    //std::vector<std::shared_ptr<UserWidget>> Actors;

    //Tick计时器
    GCPtr<Timer> tick_timer;
    GCPtr<Timer> consume_timer;
    TimerSystem timer_system;


    //类型转换
    ConverterRegistry reg;

	//组件
	GCPtr<GameWorld> game_world;
private:
    //只放全局变量初始化
    GameEngine();

public:
    static GameEngine& Instance()
    {
        static GameEngine instance;
        return instance;
    }
	void Tick();
    void Construct() override;
	void EventBegin();
    void Quit()
    {
        running = false;
    }
    ~GameEngine();
//Get
    Vec2d<double> GetViewportSize() { return SysConfig.ViewportSize; }
    SDL_Renderer* GetRenderer() { return renderer; }

    GCPtr<GameWorld> GetGameWorld();
//Render
	void RenderTexture(GCPtr<StaticTexture> texture, SDL_FRect location);

    //Sys
    void GCMark(GCObject* gc_object);

    int GCSweep();
};
