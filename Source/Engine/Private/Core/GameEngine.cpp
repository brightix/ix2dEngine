#include "Classes/Core/GameEngine.hpp"

#include "Classes/Controller.hpp"
#include "System/Font.hpp"
#include "Classes/Core/GameWorld.hpp"
#include "Classes/SubSystem/GarbageCollection.hpp"
#include "Classes/SubSystem/Sub/SubsystemManager.hpp"
#include "Public/TestPawn.hpp"
#include "Types/RenderData.hpp"
#include "Utilities/TracingUtility.hpp"

GameEngine::GameEngine() : delta_time(-1.0), GCRoot(this), engine_subsystem(nullptr), random_utility(nullptr),
                           GCSys(nullptr)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		LogWithLevel(LogLevel::FatalError,"SDL_Init Error");
		return;
	}
	timeBeginPeriod(1);
	window = SDL_CreateWindow(
		"Hello SDL3", // 标题
		1200, 1000, // 宽高
		SDL_WINDOW_RESIZABLE // 可拉伸
	);
	if (!window)
	{
		Log("SDL_CreateWindow Error:" + std::string(SDL_GetError()));
		SDL_Quit();
		return;
	}
	renderer = SDL_CreateRenderer(window, nullptr);
	std::cout << SDL_GetRendererName(renderer) << std::endl;
	if (!renderer)
	{
		Log("SDL_CreateRenderer Error: " + std::string(SDL_GetError()));
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	font_manager = &FontRenderer::Instance();
}

void GameEngine::Construct()
{
	Object::Construct();
	engine_subsystem = NewObject<SubSystemManager>(this);
	renderer_center = engine_subsystem->CreateSubsystem<RendererCenter>();

	//将自己添加进全局GC
	GCAllObjects.emplace_back(this);
	nlohmann::json j;
	std::ifstream in("Source/Engine/Config/EngineConfig.json");
	in >> j;
	in.close();

	SysConfig = {j["TargetFps"], {j["ScreenWidth"], j["ScreenHeight"]}};


	tick_timer = NewObject(new NewTimer(),this);
	consume_timer = NewObject(new NewTimer(),this);



	//Engine子系统
	GCSys = engine_subsystem->CreateSubsystem<GarbageCollection>();
	physicsSys = engine_subsystem->CreateSubsystem<SPhysics>();
	random_utility = engine_subsystem->CreateSubsystem<RandomUtility>();
	//random_utility->SetSeed(123456);
	texture_store = engine_subsystem->CreateSubsystem<TextureStoreSubSystem>();

	engine_subsystem->ForAllSubSystemInit();

	//加载默认关卡
	OnChangeWorld(new GameWorld());
}


void GameEngine::EventBegin()
{
	random_utility->RegisterRandom("SPhysicsBaseUtility_quality",{10,100});
	timer_system.SetTimer(500,[gc = GCPtr(engine_subsystem->GetSubsystem<GarbageCollection>())]() {
		int cnt{};
		if (auto p = gc.Peek())
		{
			cnt = p->GCSweep();
		}
		std::cout << "{ " << cnt << " } objects Swept!" << std::endl;
		return 5000;
	});
	//世界的事件开始

	game_world->StartSimulation();

	//最后绑定事件
	auto tick_sys = game_world->tick_SubSystem.Get();
	renderer_center->ListenDispatcher(tick_sys, "OnRenderSceneDataReady", renderer_center->ValidEvent("RenderSceneDataReady"));
	renderer_center->ListenDispatcher(tick_sys, "OnRenderWidgetDataReady", renderer_center->ValidEvent("RenderWidgetDataReady"));
	renderer_center->ListenDispatcher(tick_sys, "OnRenderClear", renderer_center->ValidEvent("RenderClear"));
	renderer_center->ListenDispatcher(tick_sys, "OnRenderPresent", renderer_center->ValidEvent("RenderPresent"));
}




void GameEngine::Tick()
{
	TStart;
	tick_timer->Start(); // 关键：第一次先 Start

	while (running) {
		consume_timer->Start();
		delta_time = tick_timer->Click();       // 重置计时


		// 场景逻辑
		game_world->Tick(delta_time);
		timer_system.Run();

		TracingUtility::ReportPerformance(GetEngineAttribution());


		//std::cout << "一怒之下tick了一下" << std::endl;
		//double fps = 1.0 / delta_time;
		//FontRenderer::Instance().UpdateTextTexture(fpsTex->GetTexture().get(), std::to_string(fps));

		//主线程查看回调函数 通知任务完成

		// 控制帧率
		//查看是否有需要处理的事件
		tick_timer->Delay(1.0 / SysConfig.TargetFps - consume_timer->End());
	}
	game_world->WorldDestroy();
	TEnd;
}


GameEngine::~GameEngine()
{
	// 清理
	if (game_world)
	{
		game_world->Unload();
	}
	GCSys->GCSweep();
	for (auto& p : GlobalPtr)
	{
		p->Reset();
	}
	GCSys->GCSweep();

	Quit();
	SDL_GetError();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	timeEndPeriod(1);
	std::cout << "程序安全退出" << std::endl;
}

GameWorld *GameEngine::GetGameWorld() const
{
	return game_world.Get();
}

PanelSlot* GameEngine::AddWidgetToViewport(Widget* widget) const
{
	//auto ret = NewObject(widget);
	//widgets.emplace(ret);
	return viewport->AddChild(widget);
}

EngineState GameEngine::GetEngineAttribution() const
{
	EngineState engine_state;
	SDL_GetWindowSize(window, &engine_state.ScreenSize.x, &engine_state.ScreenSize.y);
	if (delta_time == -1)
	{
		return engine_state;
	}
	engine_state.DeltaTime = delta_time;
	//engine_state.FPS = 1.0 / delta_time;
	return engine_state;
}

GCPtr<SubSystemManager> GameEngine::GetEngineSubSystemManager() const
{
	return engine_subsystem;
}

GCObject *GameEngine::GetGCRoot() const
{
	return GCRoot;
}

void GameEngine::Stop()
{
	running = false;
}

void GameEngine::OnChangeWorld(GameWorld* new_world)
{
	game_world = NewObject(new_world,this);
	game_world->ConstructWorld();
	//tick管理器 绑定到 渲染线程
}
std::shared_ptr<SDL_Texture> GameEngine::GetDefaultTexture() const
{
	return renderer_center->DefaultTexture;
}

void GameEngine::Quit() const
{
	for (int i = 0;i<GCAllObjects.size(); ++i)
	{
		if (GCAllObjects[i] == this || GCAllObjects[i] == nullptr)
		{
			continue;
		}
		if (i == 29)
		{
			std::cout << "1";
		}
		delete GCAllObjects[i];
		GCAllObjects[i] = nullptr;
	}
}



//传给世界来做
PanelSlot* AddToViewport(Widget* new_widget)
{
	return World()->AddToViewport(new_widget);
}


GameWorld *World()
{
	return GameEngine::Instance().GetGameWorld();
}

GameEngine& Engine()
{
	return GameEngine::Instance();
}

std::shared_ptr<SDL_Texture> Create_OutLineTexture_S(const Vec2<float>& size, SDL_Color color)
{
	return GameEngine::Instance().renderer_center->CreateOutLineTexture(size,color);
}

std::shared_ptr<SDL_Texture> Create_FilledTexture_S(const Vec2<float>& size, SDL_Color color)
{
	return GameEngine::Instance().renderer_center->CreateFilledTexture(size,color);
}

