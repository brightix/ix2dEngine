#pragma once
#include <vector>

#include "Classes/Actor.h"
#include "Classes/GameModeBase.hpp"
#include "Classes/Widget/CanvasWidget.hpp"
#include "Utilities/Timer.hpp"


class GameWorld : public Object
{

	std::vector<Actor*> actors;
	//游戏模式
	GCPtr<GameModeBase> game_mode;


	//
	bool is_simulation;
	//调试面板
	GCPtr<CanvasWidget> debug_viewport;
	GCPtr<Timer> GC_timer;
public:
    GameWorld();
    ~GameWorld()= default;
	void Construct() override;
	void StartSimulation();
	// 从类构建Actor
	// void SpawnActorFromClass(std::shared_ptr<T> actor){
	// 	Actors.push_back(actor);
	// }

	//从类构建对象
	template<typename T,typename...Args>
	GCPtr<T> SpawnActorFromClass(Args...args)
	{
		GCPtr<T> i = make_GCPtr<T>(std::forward<Args>(args)...);
		auto a = static_cast<Actor*>(i.Get());
		a->Construct();
		actors.emplace_back(a);
		return i;
	}
	template<typename T>
	GCPtr<T> SpawnActorFromClass(T* actor_raw)
	{
		auto i = GCPtr<T>(actor_raw, this);
		auto a = static_cast<Actor*>(i.Get());
		a->Construct();
		AddToWorld(a);
		return i;
	}


//Sys
	void AddToWorld(Actor* actor);
	//Debug限定
	void PrintString(std::string, int exist_time, SDL_Color color = {0, 185, 247,255});

	void Tick(double deltaTime);
};

