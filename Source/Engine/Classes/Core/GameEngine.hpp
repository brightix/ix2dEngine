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

	//子系统
	GCPtr<SubSystemManager> engine_subsystem;
	GCPtr<CanvasWidget> viewport;
	std::unordered_set<GCPtr<Widget>> widgets;
	GCWeakPtr<RandomUtility> random_utility;
    GCWeakPtr<GarbageCollection> GCSys;

private:
    //只放全局变量初始化
    GameEngine();
public:
	SDL_Renderer * renderer;
	SDL_Window * window;
	GCWeakPtr<RendererCenter> renderer_center;
	FontRenderer* font_manager;
	GCWeakPtr<TickSubSystem> tick_SubSystem;
	GCWeakPtr<TextureStoreSubSystem> texture_store;
    TimerSystem timer_system;
	GCWeakPtr<SPhysics> physicsSys;
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

    std::shared_ptr<SDL_Texture> GetDefaultTexture();

    void Quit() const;

    ~GameEngine() override;
//base
    //SDL_Renderer* GetRenderer() { return renderer; }
    GCWeakPtr<GameWorld> GetGameWorld();
    GCObject *GetGCRoot() const;
//Render
	//void RenderTexture(GCPtr<Texture> texture, SDL_FRect location);

    //Sys

//Widget

	//GCWeakPtr<CanvasWidget> GetViewport();
	Vec2<int> GetViewportSize() { return SysConfig.ViewportSize; }

    GCWeakPtr<PanelSlot> AddWidgetToViewport(GCPtr<Widget> widget) const;

    //属性
	EngineState GetEngineAttribution();

    GCWeakPtr<SubSystemManager> GetEngineSubSystemManager();
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

GCWeakPtr<PanelSlot> AddToViewport(GCPtr<Widget> new_widget);

inline std::shared_ptr<SDL_Texture> GetDefaultTexture()
{
	return GameEngine::Instance().renderer_center->DefaultTexture;
}

GCWeakPtr<GameWorld> World();
GameEngine& Engine();
std::shared_ptr<SDL_Texture> Create_OutLineTexture_S(const Vec2<float>& size, SDL_Color color = RED);
std::shared_ptr<SDL_Texture> Create_FilledTexture_S(const Vec2<float>& size, SDL_Color color = EmeraldGreen);