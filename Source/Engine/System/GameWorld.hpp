#pragma once
#include <vector>

#include "Classes/Actor.hpp"
#include "Classes/GameModeBase.hpp"
#include "Classes/Core/ThreadManager.hpp"
#include "Classes/Core/TickManager.hpp"
#include "Classes/Widget/CanvasWidget.hpp"
#include "Utilities/Timer.hpp"


class TickManager;

class GameWorld : public Object
{

	std::vector<Actor*> actors;
	//游戏模式
	GCPtr<GameModeBase> game_mode;


	TickManager tick_manager;
	ThreadManager thread_manager;
	//
	bool is_simulation;
	bool is_server;
	//调试面板
	GCPtr<CanvasWidget> debug_viewport;
	GCPtr<Timer> GC_timer;

	std::vector<GCPtr<Controller>> controllers;
public:
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
	std::vector<Actor*> GetActors();

//Set
	void RemoveActorByPtr(Actor* actor);
// 服务器方法
	//添加玩家
	GCPtr<Controller> AddController(Controller *controller);

//Sys
	void AddToWorld(Actor* actor);
	//Debug限定
	void PrintString(std::string, int exist_time, SDL_Color color = {0, 185, 247,255});

	void Tick(double delta_time);

	bool IsServer() const;
	bool IsClient() const;

	//从类构建对象
	template<typename T,typename...Args>
	GCPtr<T> SpawnActorToWorld(Args...args)
	{
		GCPtr<T> i = make_GCPtr<T>(std::forward<Args>(args)...);
		auto a = static_cast<Actor*>(i.Get());
		a->Construct();
		actors.emplace_back(a);
		return i;
	}
	template<typename T>
	GCPtr<T> SpawnActorToWorld(T* actor_raw)
	{
		auto i = GCPtr<T>(actor_raw, this);
		auto a = static_cast<Actor*>(i.Get());
		a->Construct();
		actors.emplace_back(a);
		//AddToWorld(a);
		return i;
	}

};


