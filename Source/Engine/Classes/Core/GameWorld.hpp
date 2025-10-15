#pragma once
#include <vector>

#include "GameEngine.hpp"
#include "Classes/Actor.hpp"
#include "Classes/Core/ThreadManager.hpp"
#include "Classes/Core/TickManager.hpp"
#include "../SubSystem/Sub/WorldSubSystem.hpp"
#include "Classes/Widget/CanvasWidget.hpp"
#include "Utilities/Timer.hpp"
#include "Utilities/FuncLib/SystemLib.hpp"
#include "Classes/Core/TimerSystem.hpp"
#include "Classes/SubSystem/Sub/SubsystemManager.hpp"

class Controller;
class GameModeBase;
class TickManager;
class EngineSubSystem;
class GameWorld : public Object
{
	std::vector<GCPtr<Actor>> actors;
	//游戏模式
	GCPtr<GameModeBase> game_mode;


	TickManager tick_manager;
	ThreadManager thread_manager;
	//
	bool is_server;
	//调试面板
	GCPtr<CanvasWidget> debug_viewport;
	GCPtr<Timer> GC_timer;

	std::vector<GCPtr<Controller>> controllers;

	//子系统
	TimerSystem timer_system;
	SubsystemManager<WorldSubSystem> world_subsystem;
public:

	SPhysics physicsSys;
	bool is_simulation;

    GameWorld();
    ~GameWorld()= default;
	void Construct() override;
	void StartSimulation();
	// 从类构建Actor
	// void SpawnActorFromClass(std::shared_ptr<T> actor){
	// 	Actors.push_back(actor);
	// }


//Get
	std::vector<GCPtr<Controller>> GetControllers();
	Controller* GetController(int id);
	std::vector<GCPtr<Actor>> &GetActors();

	void RemoveActorByGCPtr(GCPtr<Actor> &actor);

	//Set
	//void RemoveActorByPtr(Actor* actor);


// 服务器方法
	//添加玩家
	GCPtr<Controller> AddController(Controller *controller);

//Sys
	void AddToWorld(GCPtr<Actor> actor);
	//Debug限定
	void PrintString(std::string, int exist_time, SDL_Color color = {0, 185, 247,255});

	void Tick(double delta_time);
	//作用在切换关卡
	void WorldDestroy();
	bool IsServer() const;
	bool IsClient() const;


//Utility
	void RegisterToSPhysics(SPhysicsBaseUtility* obj);


	// //从类构建对象
	// template<typename T,typename...Args>
	// GCPtr<T> SpawnActor(Args...args)
	// {
	// 	GCPtr<T> gc = make_GCPtr<T>(std::forward<Args>(args)...);
	// 	actors.emplace_back(gc);
	// 	auto a = static_cast<Actor*>(gc.Get());
	// 	a->Construct();
	// 	return a;
	// }
	// template<typename T>
	// GCPtr<T> SpawnActor(T* actor_raw)
	// {
	// 	auto i = GCPtr<T>(actor_raw, this);
	// 	auto a = static_cast<Actor*>(i.Get());
	// 	a->Construct();
	// 	actors.emplace_back(a);
	// 	if (is_simulation)
	// 	{
	// 		a->EventBegin();
	// 	}
	// 	else
	// 	{
	// 		dispatcher_system.BindEventTo("EventBegin",a,Event("EventBegin",[a](TEventParams) {
	// 			a->EventBegin();
	// 		}));
	// 	}
	// 	return i;
	// }

};

inline GameWorld* GetWorld()
{
	return GameEngine::Instance().GetGameWorld().Get();
}

template<typename T>
GCPtr<T> SpawnActor(T* actor)
{
	static_assert(std::is_base_of_v<Actor,T>, "SpawnActor的返回值必须继承自Actor");
	GCPtr<T> a = GCPtr<T>(actor,GetWorld());
	actor->Construct();
	GameWorld* world = GetWorld();
	if (world->is_simulation)
	{
		a->EventBegin();
	}
	else
	{
		world->dispatcher_system.BindEventTo("EventBegin",a.Get(),Event("EventBegin",[a](TEventParams) {
			a->EventBegin();
		}));
	}
	GetWorld()->AddToWorld(a);
	return a;
}


