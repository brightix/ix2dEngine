#pragma once

#include <SDL3/SDL.h>

#include "RendererCenter.hpp"
#include "Classes/Core/TimerSystem.hpp"
#include "Classes/SubSystem/GarbageCollection.hpp"
#include "Utilities/Timer.hpp"
#include "Structure/SystemConfig.hpp"
#include "Utilities/ExternalWrapper.hpp"
#include "Utilities/GCPtr.hpp"
#include "Classes/SubSystem/TextureStoreSubSystem.hpp"
#include "Classes/SubSystem/Sub/SubsystemManager.hpp"
#include "System/Font.hpp"
#include "Types/EngineState.hpp"
#include "Utilities/RandomUtility.hpp"

class SPhysics;
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
	GCPtr<CanvasWidget> viewport;

	//子系统
	GCStrongPtr<SubSystemManager> engine_subsystem;
	RandomUtility* random_utility;
    GarbageCollection* GCSys;

private:
    //只放全局变量初始化
    GameEngine();
public:
	SDL_Renderer * renderer;
	SDL_Window * window;
	GCPtr<RendererCenter> renderer_center;
	FontRenderer* font_manager;
	GCPtr<TextureStoreSubSystem> texture_store;
    TimerSystem timer_system;
	SPhysics* physicsSys;
public:

    static GameEngine& Instance()
    {
        static GameEngine instance;
        return instance;
    }
	void Tick();
    void Construct() override;
	void EventBegin();
    void Stop();

    void OnChangeWorld(GameWorld *new_world);

    std::shared_ptr<SDL_Texture> GetDefaultTexture() const;

    void Quit() const;

    ~GameEngine() override;
//base
    //SDL_Renderer* GetRenderer() { return renderer; }
    GameWorld *GetGameWorld() const;
    GCObject *GetGCRoot() const;
	double GetDeltaTime() const;
//Render
	//void RenderTexture(GCPtr<Texture> texture, SDL_FRect location);

    //Sys

//Widget

	//GCPtr<CanvasWidget> GetViewport();
	Vec2<int> GetViewportSize() { return SysConfig.ViewportSize; }

    PanelSlot *AddWidgetToViewport(Widget *widget) const;

    //属性
	EngineState GetEngineAttribution() const;

    SubSystemManager* GetEngineSubSystemManager() const;


	//子系统
	template<typename T>
	T* CreateEngineSubsystem()
	{
		if (!engine_subsystem)
		{
			return nullptr;
		}
		return engine_subsystem->CreateSubsystem<GarbageCollection>(this);
	}
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

inline bool IsValid(const GCObject* obj)
{
	return IsValid(obj->id);
}

PanelSlot* AddToViewport(Widget* new_widget);

inline std::shared_ptr<SDL_Texture> GetDefaultTexture()
{
	return GameEngine::Instance().renderer_center->DefaultTexture;
}


GameWorld *World();
GameEngine& Engine();
std::shared_ptr<SDL_Texture> Create_OutLineTexture_S(const Vec2<float>& size, SDL_Color color = RED);
std::shared_ptr<SDL_Texture> Create_FilledTexture_S(const Vec2<float>& size, SDL_Color color = EmeraldGreen);

double GetDeltaTime();