#pragma once
#include <vector>

#include "Classes/Actor.hpp"
#include "Classes/GameModeBase.hpp"
#include "Classes/Core/ThreadManager.hpp"
#include "Classes/Core/TickManager.hpp"
#include "Classes/SubSystem/WorldSubSystem.hpp"
#include "Classes/Widget/CanvasWidget.hpp"
#include "Utilities/Timer.hpp"
#include "Utilities/FuncLib/SystemLib.hpp"
#include "Classes/Core/TimerSystem.hpp"

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

	//子系统
	TimerSystem timer_system;
	std::unordered_map<uint32_t,GCPtr<WorldSubSystem>> world_subSystem;
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
	//作用在切换关卡
	void WorldDestroy();
	bool IsServer() const;
	bool IsClient() const;












	template<typename T>
	void CreateWorldSubSystem(const std::string& subSys_name)
	{
		static_assert(std::is_base_of_v<WorldSubSystem, T>, "T 必须继承自 WorldSubSystem");
		const uint32_t id = ix::Hash(subSys_name.c_str());
		if (!world_subSystem.contains(id))
		{
			world_subSystem[id] = make_GCPtr<T>();
		}
		else
		{
			Log("重复添加子系统");
		}
	}
	WorldSubSystem* GetWorldSubSystem(const std::string& subSys_name)
	{
		uint32_t id = ix::Hash(subSys_name.c_str());
		if (world_subSystem.contains(id))
		{
			return world_subSystem[id].Get();
		}
		return nullptr;
	}


	//从类构建对象
	template<typename T,typename...Args>
	GCPtr<T> SpawnActor(Args...args)
	{
		GCPtr<T> i = make_GCPtr<T>(std::forward<Args>(args)...);
		auto a = static_cast<Actor*>(i.Get());
		a->Construct();
		actors.emplace_back(a);
		return i;
	}
	template<typename T>
	GCPtr<T> SpawnActor(T* actor_raw)
	{
		auto i = GCPtr<T>(actor_raw, this);
		auto a = static_cast<Actor*>(i.Get());
		a->Construct();
		actors.emplace_back(a);
		if (is_simulation)
		{
			a->EventBegin();
		}
		else
		{
			dispatcher_system.BindEventTo("EventBegin",a,Event("EventBegin",[a](TEventParams) {
				a->EventBegin();
			}));
		}
		return i;
	}

};




