#pragma once

#include <SDL3/SDL.h>
#include "../Component/SenceComponent/Texture.hpp"
#include "Classes/Core/TimerSystem.hpp"
#include "Utilities/Timer.hpp"
#include "Structure/SystemConfig.hpp"
#include "Utilities/ConverterRegistry.hpp"
#include "Utilities/ExternalWrapper.hpp"
#include "Utilities/GCPtr.hpp"
#include "Classes/SubSystem/GarbageCollection.hpp"
#include "Classes/SubSystem/Sub/SubsystemManager.hpp"

class GameWorld;

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
    GCPtr<NewTimer> tick_timer;
    GCPtr<NewTimer> consume_timer;


    //类型转换
    ConverterRegistry reg;

    mutable GCObject* GCRoot;
	//组件
	GCPtr<GameWorld> game_world;

	//子系统
	GCPtr<SubsystemManager<EngineSubSystem>> engine_subsystem;
private:
    //只放全局变量初始化
    GameEngine();
public:

    TimerSystem timer_system;
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
    ~GameEngine() override;
//Get
    Vec2<double> GetViewportSize() { return SysConfig.ViewportSize; }
    SDL_Renderer* GetRenderer() { return renderer; }
    GCPtr<GameWorld> GetGameWorld();

    GCObject *GetGCRoot() const;
//Render
	void RenderTexture(GCPtr<Texture> texture, SDL_FRect location);

    //Sys
};




inline const GameEngine* GetEngine()
{
	return &GameEngine::Instance();
}

inline const SDL_Renderer* GetRenderer();