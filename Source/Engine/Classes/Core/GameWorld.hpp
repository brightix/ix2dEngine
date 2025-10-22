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

class ViewportSubSystem;
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
	GCPtr<SubSystemManager> world_subsystem;
	GCWeakPtr<ViewportSubSystem> viewport;



	std::set<GCPtr<Widget>,GCPtrLess> widgets;
public:
	GCWeakPtr<TickSubSystem> tick_SubSystem;
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

	std::vector<GCPtr<Actor>> *GetActors();

	std::vector<GCWeakPtr<Widget>> GetWidgets();
	void RemoveActorByGCPtr(const GCPtr<Actor> &actor);

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


	//Widget
	GCWeakPtr<Widget> AddToViewport(Widget *w);
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
		world->dispatcher_system.BindEventTo(a.Get(), "EventBegin",Event("EventBegin",[a](TEventParams) {
			a->EventBegin();
		}));
	}
	GetWorld()->AddToWorld(a);
	return a;
}


