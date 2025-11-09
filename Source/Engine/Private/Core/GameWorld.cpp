#include "../../Classes/Core/GameWorld.hpp"
#include "../../Classes/Core/GameEngine.hpp"
#include "Classes/Character.hpp"
#include "public/TestActor.h"
#include "Classes/GameModeBase.hpp"
#include "Classes/SubSystem/ViewportSubSystem.hpp"
#include "Public/TestFpsWidget.hpp"
#include "Classes/Widget/PanelWidget/CanvasWidget.hpp"
#include "Enum/ActorEnum.hpp"
#include "Public/TestGround.hpp"
#include "public/TestPawn.hpp"

GameWorld::GameWorld() : is_server(false), is_simulation(false) ,world_subsystem(nullptr)
{
	CNAME;
}

GameWorld::~GameWorld()
{

}

void GameWorld::Unload() const
{
	for (auto& actor : actors)
	{
		actor->is_pending_kill = true;
	}
	viewport->is_pending_kill = true;
}

void GameWorld::Construct()
{
	Object::Construct();
}

void GameWorld::RegisterDispatchers()
{
	Object::RegisterDispatchers();
	AddDispatcher("OnWorldEventBegin");
}

void GameWorld::ConstructWorld()
{
	is_server = true;

	world_subsystem = NewObject<SubSystemManager>(this);
	//tick管理器
	tick_SubSystem = world_subsystem->CreateSubsystem<TickSubSystem>();
	tick_SubSystem->SetBufferType(1);
	tick_SubSystem->dispatcher_system.AddDispatcher("synchronization");//可以加在该子系统的构造函数内
	// ================== 包围整个场景 ==================
	const auto ground = SpawnActor<TestGround>(Transform(Vec2<float>(-100, 1000 - 100)));
	ground->SetActorName("Ground");
	ground->SetGroundSize(Vec2<float>(1200, 200)); // 比场景略宽，防止掉出边界

	// const auto roof = SpawnActor<TestGround>(Transform(Vec2<float>(-100, -100)));
	// roof->SetActorName("Roof");
	// roof->SetGroundSize(Vec2<float>(1200, 200)); // 顶部边界
	//
	// const auto left_wall = SpawnActor<TestGround>(Transform(Vec2<float>(-100, -100)));
	// left_wall->SetActorName("LeftWall");
	// left_wall->SetGroundSize(Vec2<float>(200, 1200)); // 左边界
	//
	// const auto right_wall = SpawnActor<TestGround>(Transform(Vec2<float>(1000 - 100, -100)));
	// right_wall->SetActorName("RightWall");
	// right_wall->SetGroundSize(Vec2<float>(200, 1200)); // 右边界
	//
	// // ================== 以下为复杂地形 ==================
	//
	// // 主地面（起点区域）
	// const auto ground1 = SpawnActor<TestGround>(Transform(Vec2<float>(0, 900)));
	// ground1->SetGroundSize(Vec2<float>(400, 100));
	//
	// // 中段地形：略微抬升
	// const auto ground2 = SpawnActor<TestGround>(Transform(Vec2<float>(400, 850)));
	// ground2->SetGroundSize(Vec2<float>(300, 80));
	//
	// // 右侧下沉地形
	// const auto ground3 = SpawnActor<TestGround>(Transform(Vec2<float>(700, 920)));
	// ground3->SetGroundSize(Vec2<float>(300, 80));
	//
	// // ================== 中部跳跃区域 ==================
	// const auto platform3 = SpawnActor<TestGround>(Transform(Vec2<float>(400, 700)));
	// platform3->SetGroundSize(Vec2<float>(150, 50));
	//
	// const auto platform4 = SpawnActor<TestGround>(Transform(Vec2<float>(550, 650)));
	// platform4->SetGroundSize(Vec2<float>(150, 50));
	//
	// const auto platform5 = SpawnActor<TestGround>(Transform(Vec2<float>(650, 700)));
	// platform5->SetGroundSize(Vec2<float>(150, 50));
	//
	// // ================== 左上方斜坡（分段式） ==================
	// const auto slope1 = SpawnActor<TestGround>(Transform(Vec2<float>(150, 750)));
	// slope1->SetGroundSize(Vec2<float>(100, 30));
	//
	// const auto slope2 = SpawnActor<TestGround>(Transform(Vec2<float>(220, 700)));
	// slope2->SetGroundSize(Vec2<float>(100, 30));
	//
	// const auto slope3 = SpawnActor<TestGround>(Transform(Vec2<float>(290, 650)));
	// slope3->SetGroundSize(Vec2<float>(100, 30));
	//
	// // ================== 坑洞边缘 ==================
	// const auto pit_left = SpawnActor<TestGround>(Transform(Vec2<float>(350, 950)));
	// pit_left->SetGroundSize(Vec2<float>(50, 100));
	//
	// const auto pit_right = SpawnActor<TestGround>(Transform(Vec2<float>(550, 950)));
	// pit_right->SetGroundSize(Vec2<float>(50, 100));
	//
	// // ================== 高层平台（终点区域） ==================
	// const auto high_platform1 = SpawnActor<TestGround>(Transform(Vec2<float>(800, 300)));
	// high_platform1->SetGroundSize(Vec2<float>(150, 40));
	//
	// const auto high_platform2 = SpawnActor<TestGround>(Transform(Vec2<float>(900, 250)));
	// high_platform2->SetGroundSize(Vec2<float>(150, 40));
	//auto size = GameEngine::Instance().GetEngineAttribution().ScreenSize;
}

void GameWorld::StartSimulation()
{
	printf("---------------simulation---------------\n");
	viewport = CreateWidget<CanvasWidget>();
	//viewport->ConstructEvent();
	world_subsystem->ForAllSubSystemInit();
	game_mode = SpawnActor<GameModeBase>();

	auto fps = CreateWidget<TestFps>();
	AddToViewport(fps);
	//测试地面

	///FRect bound(0,0,size.x,size.y);
	FRect bound(100,100,800,10);

	nlohmann::json world_config;
	std::ifstream file = std::ifstream("Source/Engine/Config/WorldConfig.json");
	file >> world_config;
	file.close();
	RandCreateActorInBox<Character>(bound,world_config["WorldActorCnt"].get<int>());

	//RandCreateActorInBox<Actor>({0,0,10,10},100);

	is_simulation = true;
	CallDispatcher("OnWorldEventBegin");
}

std::vector<Controller*> GameWorld::GetControllers() const
{
	std::vector<Controller*> ret;
	for (auto& c : controllers)
	{
		ret.emplace_back(c.Get());
	}
	return ret;
}

Controller *GameWorld::GetController(int id) const
{
	if (id >= 0 && id < controllers.size())
	{
		return controllers[id].Get();
	}
	return nullptr;
}

std::vector<Actor*> GameWorld::GetActors() const
{
	std::vector<Actor*> ret;
	for (auto& actor : actors)
	{
		ret.emplace_back(actor.Get());
	}
	return ret;
}

std::vector<GCPtr<Widget>> GameWorld::GetWidgets() const
{
	std::vector<GCPtr<Widget>> v;
	for (auto& item : widgets)
	{
		v.emplace_back(item);
	}
	return v;
}

void GameWorld::RemoveActor(const Actor* actor)
{
	std::erase(actors, actor);
}

Controller* GameWorld::AddController()
{
	if (is_server)
	{
		auto t = SpawnActor<Controller>();
		controllers.emplace_back(t);
		return t;
	}
	return {};
}

void GameWorld::AddToWorld(Actor* actor)
{
	actors.emplace_back(actor);
}

void GameWorld::NativeWorldTick(double delta_time)
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




