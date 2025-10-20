#pragma once

#include <SDL3/SDL.h>

#include "RendererCenter.hpp"
#include "../Component/SenceComponent/Texture.hpp"
#include "Classes/Core/TimerSystem.hpp"
#include "Utilities/Timer.hpp"
#include "Structure/SystemConfig.hpp"
#include "Utilities/ConverterRegistry.hpp"
#include "Utilities/ExternalWrapper.hpp"
#include "Utilities/GCPtr.hpp"
#include "Classes/SubSystem/GarbageCollection.hpp"
#include "Classes/SubSystem/TextureStoreSubSystem.hpp"
#include "Classes/SubSystem/Sub/SubsystemManager.hpp"
#include "System/Font.hpp"

class RendererCenter;
class TickSubSystem;
class GameWorld;

class GameEngine final : public Object
{
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
	GCPtr<SubSystemManager> engine_subsystem;
private:
    //只放全局变量初始化
    GameEngine();
public:
	RendererCenter* renderer_center;
	FontRenderer* font_manager;
	GCWeakPtr<TickSubSystem> tick_SubSystem;
	TextureStoreSubSystem* texture_store;
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
    void Quit();
    ~GameEngine() override;
//Get
    Vec2<double> GetViewportSize() { return SysConfig.ViewportSize; }
    //SDL_Renderer* GetRenderer() { return renderer; }
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

inline SDL_Renderer* GetRenderer()
{
	return GameEngine::Instance().renderer_center->GetRenderer();
}

inline bool IsValid(const size_t id)
{
	return Global_GCObject_Registry.contains(id) && !Global_GCObject_Registry[id]->is_pending_kill;
}