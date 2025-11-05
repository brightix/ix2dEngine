#include "../../Classes/Core/GameWorld.hpp"
#include "../../Classes/Core/GameEngine.hpp"
#include "public/TestActor.h"
#include "Classes/GameModeBase.hpp"
#include "Classes/SubSystem/ViewportSubSystem.hpp"
#include "Public/TestFpsWidget.hpp"
#include "Classes/Widget/PanelWidget/CanvasWidget.hpp"
#include "Enum/ActorEnum.hpp"
#include "Public/TestGround.hpp"
#include "public/TestPawn.hpp"

GameWorld::GameWorld() : is_simulation(false), is_server(false)
{
	CNAME;
}

GameWorld::~GameWorld()
{

}

void GameWorld::Unload()
{
	for (auto& actor : actors)
	{
		actor->is_pending_kill = true;
	}
	viewport->is_pending_kill = true;
}

void GameWorld::ConstructWorld()
{
	is_server = true;

	world_subsystem = NewObject<SubSystemManager>(this);
	//窗口
	//viewport_sub = world_subsystem->CreateSubSystem<ViewportSubSystem>("ViewportSubSystem");
	//tick管理器
	tick_SubSystem = world_subsystem->CreateSubsystem<TickSubSystem>();
	tick_SubSystem->SetBufferType(1);
	tick_SubSystem->dispatcher_system.AddEventDispatcher("synchronization");//可以加在该子系统的构造函数内
}

void GameWorld::StartSimulation()
{
	printf("---------------simulation---------------\n");

	dispatcher_system.AddEventDispatcher("EventBegin");
	viewport = CreateWidget<CanvasWidget>(this);
	//viewport->ConstructEvent();
	world_subsystem->ForAllSubSystemInit();

	game_mode = SpawnActor<GameModeBase>();

	auto fps = CreateWidget(new TestFps);
	AddToViewport(fps);

 // 	auto dd = SpawnActor(new Actor(Transform{{500,500}}));
	// GameEngine::Instance().timer_system.SetTimer(1000,[dd]() {
	// 	dd->DestroyActor();
	// 	return -1;
	// });


	auto config = Engine().GetEngineAttribution();

	//测试地面
	const auto ground = SpawnActor<TestGround>(Transform(Vec2<float>(-100, config.ScreenSize.y - 100)));
	const auto roof= SpawnActor<TestGround>(Transform(Vec2<float>(-100, -900)));
	const auto left_wall= SpawnActor<TestGround>(Transform(Vec2<float>(-900, -100)));
	const auto right_wall= SpawnActor<TestGround>(Transform(Vec2<float>(config.ScreenSize.x - 100, -100)));


	auto size = GameEngine::Instance().GetEngineAttribution().ScreenSize;
	FRect bound(0,0,size.x,size.y);

	nlohmann::json world_config;
	std::ifstream file = std::ifstream("Source/Engine/Config/WorldConfig.json");
	file >> world_config;
	file.close();
	RandCreateActorInBox<TestPawn>(bound,world_config["WorldActorCnt"].get<int>());
	//RandCreateActorInBox<Actor>({0,0,10,10},100);


	is_simulation = true;
	dispatcher_system.CallDispatcher("EventBegin");

//在真正Begin后才建议设置对象属性
	//ground->SetMobility(ActorMobility::Static);
}

std::vector<GCPtr<Controller>> GameWorld::GetControllers()
{
	return controllers;
}

GCPtr<Controller> GameWorld::GetController(int id) const
{
	if (id >= 0 && id < controllers.size())
	{
		return controllers[id];
	}
	return {};
}

std::vector<GCPtr<Actor>> *GameWorld::GetActors()
{
	return &actors;
}

std::vector<GCPtr<Widget>> GameWorld::GetWidgets()
{
	std::vector<GCPtr<Widget>> v;
	for (auto& item : widgets)
	{
		v.emplace_back(item);
	}
	return v;
}

void GameWorld::RemoveActorByGCPtr(const GCPtr<Actor>& actor)
{
	std::erase(actors, actor);
}

GCPtr<Controller> GameWorld::AddController(Controller *controller)
{
	if (is_server)
	{
		GCPtr<Controller> t = SpawnActor<Controller>(controller);
		controllers.push_back(t);
		return t;
	}
	return {};
}

void GameWorld::AddToWorld(Actor* actor)
{
	actors.emplace_back(actor);
}

void GameWorld::Tick(double delta_time)
{
	if (!is_simulation)
	{
		return;
	}
	timer_system.Run();
	//一般情况下不使用game_mode的tick
	game_mode->Tick(delta_time);
	tick_SubSystem->Tick(delta_time);
}

void GameWorld::WorldDestroy() const
{
	for (const auto subsystem = world_subsystem->GetAllSubSystem(); auto& it : subsystem)
	{
		it->DeInit();
	}
}

bool GameWorld::IsServer() const
{
	return is_server;
}

bool GameWorld::IsClient() const
{
	return !is_server;
}

PanelSlot* GameWorld::AddToViewport(Widget* w) const
{
	return viewport->AddChild(w);
}




