#include "../../Classes/Core/GameEngine.hpp"

#include "System/Font.hpp"
#include "Classes/Core/GameWorld.hpp"
#include "Classes/SubSystem/GarbageCollection.hpp"
#include "Classes/SubSystem/Sub/SubsystemManager.hpp"
#include "Types/RenderData.hpp"

GameEngine::GameEngine() : delta_time(0), GCRoot(this)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		LogWithLevel("SDL_Init Error", LogLevel::FatalError);
		return;
	}
	timeBeginPeriod(1);
	window = SDL_CreateWindow(
		"Hello SDL3",        // 标题
		1200, 1000,            // 宽高
		SDL_WINDOW_RESIZABLE // 可拉伸
		);
		if (!window)
		{
			Log("SDL_CreateWindow Error:" + std::string(SDL_GetError()));
			SDL_Quit();
			return;
		}
		renderer = SDL_CreateRenderer(window, nullptr);
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

	//最先启动GPU
	// int count = SDL_GetNumRenderDrivers();
	// for (int i = 0; i < count; ++i) {
	// 	const char* name = SDL_GetRenderDriver(i);
	// 	SDL_Log("Render driver[%d]: %s", i, name);
	// }
	engine_subsystem = NewObject<SubSystemManager>(new SubSystemManager());
	renderer_center = engine_subsystem->CreateSubSystem<RendererCenter>("RendererCenter");
	//renderer_center->SetRendererAndWindow(renderer,window);

	//renderer_center->StartRenderThread();

	//将自己添加进全局GC
	GCAllObjects.emplace_back(this);
	SysConfig = {120, {640, 480}};


	tick_timer = NewObject(new NewTimer());
	consume_timer = NewObject(new NewTimer());




	//Engine子系统
	GCSys = engine_subsystem->CreateSubSystem<GarbageCollection>("GarbageCollection");
	random_utility = engine_subsystem->CreateSubSystem<RandomUtility>("RandomUtility");
	//random_utility->SetSeed(123456);
	texture_store = engine_subsystem->CreateSubSystem<TextureStoreSubSystem>("TextureStoreSubSystem");


	//加载默认关卡
	OnChangeWorld(new GameWorld());
}


void GameEngine::EventBegin()
{
	engine_subsystem->ForAllSubSystemInit();
	GCWeakPtr<GarbageCollection> gc = engine_subsystem->GetSubSystem<GarbageCollection>("GarbageCollection");
	timer_system.SetTimer(500,[gc]() {
		int cnt{};
		if (auto p = gc.Peek())
		{
			cnt = p->GCSweep();
		}
		std::cout << "{ " << cnt << " } objects have been Swept!" << std::endl;
		return 2000;
	});
	//世界的事件开始

	game_world->StartSimulation();

	//最后绑定事件
	game_world->tick_SubSystem->BindEvent(renderer_center.ptr, "RenderSceneDataReady", *renderer_center->event_system.GetEventByName("OnRenderSceneDataReady"));
	game_world->tick_SubSystem->BindEvent(renderer_center.ptr, "RenderWidgetDataReady", *renderer_center->event_system.GetEventByName("OnRenderWidgetDataReady"));
	game_world->tick_SubSystem->BindEvent(renderer_center.ptr, "RenderClear", *renderer_center->event_system.GetEventByName("OnRenderClear"));
	game_world->tick_SubSystem->BindEvent(renderer_center.ptr, "RenderPresent", *renderer_center->event_system.GetEventByName("OnRenderPresent"));

}




void GameEngine::Tick()
{
	tick_timer->Start(); // 关键：第一次先 Start

	while (running) {
		consume_timer->Start();
		delta_time = tick_timer->Click();       // 重置计时


		// 场景逻辑
		game_world->Tick(delta_time);
		timer_system.Run();
		//std::cout << "一怒之下tick了一下" << std::endl;
		//double fps = 1.0 / delta_time;
		//FontRenderer::Instance().UpdateTextTexture(fpsTex->GetTexture().get(), std::to_string(fps));

		//主线程查看回调函数 通知任务完成

		// 控制帧率
		//查看是否有需要处理的事件
		tick_timer->Delay(1.0 / SysConfig.TargetFps - consume_timer->End());
	}
	game_world->WorldDestroy();
}


GameEngine::~GameEngine()
{
	// 清理
	if (game_world)
	{
		game_world->Unload();
	}
	GCSys->GCSweep();
	if (engine_subsystem)
	{
		engine_subsystem->DeInitAllSubSystem();
	}
	timeEndPeriod(1);

	Quit();
	SDL_GetError();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Sleep(10);
	SDL_Quit();
	std::cout << "程序退出" << std::endl;
}

GCWeakPtr<GameWorld> GameEngine::GetGameWorld()
{
	return game_world;
}

GCWeakPtr<PanelSlot> GameEngine::AddWidgetToViewport(GCPtr<Widget> widget) const
{
	//auto ret = NewObject(widget);
	//widgets.emplace(ret);
	return viewport->AddChild(widget);
}

EngineState GameEngine::GetEngineAttribution()
{
	EngineState engine_state;
	engine_state.DeltaTime = delta_time;
	SDL_GetWindowSize(renderer_center->window, &engine_state.ScreenSize.x, &engine_state.ScreenSize.y);
	return engine_state;
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
	game_world = NewObject<GameWorld>(new_world);
	game_world->ConstructWorld();
	//tick管理器 绑定到 渲染线程

}
std::shared_ptr<SDL_Texture> GameEngine::GetDefaultTexture()
{
	return renderer_center->DefaultTexture;
}

void GameEngine::Quit()
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
GCWeakPtr<PanelSlot> AddToViewport(GCPtr<Widget> new_widget)
{
	return World()->AddToViewport(new_widget);
}
//
// GCWeakPtr<PanelSlot> AddToViewportSlot(Widget* new_widget)
// {
// 	return GameEngine::Instance().AddWidgetToViewport(new_widget);
// }

GCWeakPtr<GameWorld> World()
{
	return GameEngine::Instance().GetGameWorld();
}

SDL_Texture * Create_OutLineTexture_S(const Vec2<float>& size, SDL_Color color)
{
	return GameEngine::Instance().renderer_center->CreateOutLineTexture(size,color);
}

