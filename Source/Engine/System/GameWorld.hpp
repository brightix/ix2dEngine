#pragma once
#include <vector>

#include "Classes/Actor.h"
#include "Classes/GameModeBase.hpp"
#include "Classes/Widget/CanvasWidget.hpp"


class GameWorld : public Object
{

	std::vector<GCPtr<Actor>> actors;
	//游戏模式
	GCPtr<GameModeBase> game_mode;


	//调试面板
	GCPtr<CanvasWidget> debug_viewport;
public:
    GameWorld();
    ~GameWorld()= default;
	void Construct() override;
	// 从类构建Actor
	// void SpawnActorFromClass(std::shared_ptr<T> actor){
	// 	Actors.push_back(actor);
	// }

	//从类构建对象
	template<typename T,typename...Args>
	GCPtr<T> SpawnActorFromClass(Args...args)
	{
		GCPtr<T> actor = make_GCPtr<T>(std::forward<Args>(args)...);
		actors.emplace_back(actor);
		return actor;
	}
	template<typename T>
	GCPtr<T> SpawnActorFromClass(T* actor_raw)
	{
		GCPtr<T> actor = GCPtr<T>(actor_raw,this);
		actors.emplace_back(actor_raw);
		return actor;
	}


//Sys
	void ConstructWorld();
	void AddToWorld(GCPtr<Actor> actor);
	//Debug限定
	void PrintString(std::string, int exist_time, SDL_Color color = {0, 185, 247,255});

	void Tick(double deltaTime);
};

