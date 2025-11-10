#pragma once
#include <set>
#include <vector>

#include "GameEngine.hpp"
#include "Classes/Actor.hpp"
#include "Classes/Core/RendererCenter.hpp"
#include "Classes/Core/TickManager.hpp"
#include "../Widget/PanelWidget/CanvasWidget.hpp"
#include "Utilities/Timer.hpp"
#include "Classes/Core/TimerSystem.hpp"
#include "Classes/SubSystem/Sub/SubsystemManager.hpp"
class Controller;
class GameModeBase;
class TickSubSystem;
class EngineSubSystem;
struct GCPtrLess {
	bool operator()(const GCPtr<Widget>& a, const GCPtr<Widget>& b) const {
		return a->GetLayerId() < b->GetLayerId();
	}
};
class GameWorld : public Object
{
	std::vector<GCPtr<Actor>> actors;
	//游戏模式
	GCPtr<GameModeBase> game_mode;


	//
	bool is_server;
	//调试面板
	GCPtr<CanvasWidget> debug_viewport;
	GCPtr<Timer> GC_timer;

	std::vector<GCPtr<Controller>> controllers;

	//子系统
	TimerSystem timer_system;
	SubSystemManager* world_subsystem;



	std::set<GCPtr<Widget>,GCPtrLess> widgets;

public:
	GCPtr<TickSubSystem> tick_SubSystem;

	bool is_simulation;

	GCPtr<CanvasWidget> viewport;
    GameWorld();
    ~GameWorld() override;
	void Unload() const;

	void Construct() override;
	void RegisterDispatchers() override;

	virtual void ConstructWorld();
	virtual void StartSimulation();

//Get
	std::vector<Controller*> GetControllers() const;

	Controller *GetController(int id = 0) const;

	std::vector<Actor*> GetActors() const;

	std::vector<GCPtr<Widget>> GetWidgets() const;
	void RemoveActor(const Actor* actor);

	//Set
	//void RemoveActorByPtr(Actor* actor);


// 服务器方法
	//添加玩家
	Controller *AddController();

//Sys
	void AddToWorld(Actor *actor);
	//Debug限定
	//TODO 改名，此函数为单独被engine或父world调用
	virtual void NativeWorldTick(double delta_time);
	//作用在切换关卡
	void WorldDestroy() const;
	bool IsServer() const;
	bool IsClient() const;


	//Widget
	PanelSlot *AddToViewport(Widget *w) const;
};

template<typename T>
T* SpawnActor(T* actor)
{
	static_assert(std::is_base_of_v<Actor,T>, "SpawnActor的返回值必须继承自Actor");
	actor->Construct();
	GameWorld* world = World();
	actor->outer = world;
	if (world->is_simulation)
	{
		actor->EventBegin();
	}
	else
	{
		actor->ListenDispatcher(world,"OnWorldEventBegin","EventBegin");
	}
	World()->AddToWorld(actor);
	return actor;
}

template<typename T>
T* SpawnActor(Transform transform = {})
{
	static_assert(std::is_base_of_v<Actor,T>, "SpawnActor的返回值必须继承自Actor");
	T* actor = new T(transform);
	actor->Construct();
	GameWorld* world = World();
	actor->outer = world;
	if (world->is_simulation)
	{
		actor->EventBegin();
	}
	else
	{
		actor->ListenDispatcher(world,"OnWorldEventBegin","EventBegin");
	}
	//后面可以推进使用 level 分级
	world->AddToWorld(actor);
	return actor;
}

template<typename T>
void RandCreateActorInBox(const FRect& Box, const int n)
{
	NewTimer timer;
	static_assert(std::is_base_of_v<Actor,T>,"T must be derived from Actor");
	for (int i = 0; i < n; ++i)
	{
		TStartF("RandCreateActor");
		Transform t;
		t.location.x = Box.x + Box.w * Rand();
		t.location.y = Box.y + Box.h * Rand();
		SpawnActor<T>(t);
		//std::cout << i << "  in_delay: " << timer.Click() << std::endl;
		TEndF("RandCreateActor");
	}
}

template<typename T>
std::vector<T*> GetAllActorFromClass(const std::string& name = "")
{
	std::vector<T*> ret;
	auto actors = World()->GetActors();
	if (name.empty())
	{
		for (auto& actor : actors)
		{
			if (auto it = Cast<T>(actor))
			{
				ret.emplace_back(it);
			}
		}
		return ret;
	}
	for (auto& actor : actors)
	{
		if (auto it = Cast<T>(actor); it && actor->name == name)
		{
			ret.emplace_back(it);
		}
	}
	return ret;
}