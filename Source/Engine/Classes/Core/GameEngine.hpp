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
#include "Types/EngineState.hpp"

class CanvasWidget;
class Widget;
class RendererCenter;
class TickSubSystem;
class GameWorld;

class GameEngine final : public Object
{
    bool running = true;
    //系统数据
    SystemConfig SysConfig{};

    double delta_time;

    //Tick计时器
    GCPtr<NewTimer> tick_timer;
    GCPtr<NewTimer> consume_timer;


    mutable GCObject* GCRoot;


	//组件
	GCPtr<GameWorld> game_world;

	//子系统
	GCPtr<SubSystemManager> engine_subsystem;
	std::unordered_set<GCPtr<Widget>> widgets;
private:
    //只放全局变量初始化
    GameEngine();
public:
	GCWeakPtr<RendererCenter> renderer_center;
	FontRenderer* font_manager;
	GCWeakPtr<TickSubSystem> tick_SubSystem;
	GCWeakPtr<TextureStoreSubSystem> texture_store;
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
//base
    //SDL_Renderer* GetRenderer() { return renderer; }
    GCPtr<GameWorld> GetGameWorld();
    GCObject *GetGCRoot() const;
//Render
	void RenderTexture(GCPtr<Texture> texture, SDL_FRect location);

    //Sys

//Widget

	GCWeakPtr<CanvasWidget> GetViewport();
	Vec2<double> GetViewportSize() { return SysConfig.ViewportSize; }

    GCWeakPtr<Widget> AddWidgetToViewport(Widget *widget);
	//属性
	EngineState GetEngineAttribution() const;
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

inline GCWeakPtr<Widget> AddToViewport(Widget* new_widget);

inline std::shared_ptr<SDL_Texture> GetDefaultTexture()
{
	return GameEngine::Instance().renderer_center->DefaultTexture;
}
