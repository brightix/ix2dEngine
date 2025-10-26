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
	font_manager = &FontRenderer::Instance();
}

void GameEngine::Construct()
{
	//最先启动GPU
	engine_subsystem = NewObject<SubSystemManager>(new SubSystemManager());
	renderer_center = engine_subsystem->CreateSubSystem<RendererCenter>("RendererCenter");
	//renderer_center->StartRenderThread();

	//将自己添加进全局GC
	GCAllObjects.emplace_back(this);
	SysConfig = {120, {640, 480}};


	tick_timer = NewObject(new NewTimer());
	consume_timer = NewObject(new NewTimer());

	//Engine子系统
	engine_subsystem->CreateSubSystem<GarbageCollection>("GarbageCollection");
	random_utility = engine_subsystem->CreateSubSystem<RandomUtility>("RandomUtility");
	//random_utility->SetSeed(123456);
	texture_store = engine_subsystem->CreateSubSystem<TextureStoreSubSystem>("TextureStoreSubSystem");

	//加载默认关卡
	OnChangeWorld(new GameWorld());
}


void GameEngine::EventBegin()
{
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
	game_world->StartSimulation();
}




void GameEngine::Tick()
{
	tick_timer->Start(); // 关键：第一次先 Start

	//auto fps_surface = GetTextSurface("            ",{});
	//auto fpsTex = NewObject(new StaticTexture());
	//RendererCenter::SetTextureFromSurface(fpsTex.Get(),fps_surface);
	while (running) {
		consume_timer->Start();
		delta_time = tick_timer->Click();       // 重置计时


		// 场景逻辑
		game_world->Tick(delta_time);
		timer_system.Run();

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
	engine_subsystem->DeInitAllSubSystem();
	timeEndPeriod(1);
	SDL_Quit();
}

GCWeakPtr<GameWorld> GameEngine::GetGameWorld()
{
	return game_world;
}

GCWeakPtr<Widget> GameEngine::AddWidgetToViewport(Widget *widget)
{
	auto ret = NewObject(widget);
	widgets.emplace(ret);
	return GCWeakPtr<Widget>(ret);
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

void GameEngine::Quit()
{
	running = false;
}

void GameEngine::OnChangeWorld(GameWorld* new_world)
{
	game_world = NewObject<GameWorld>(new_world);
	//tick管理器 绑定到 渲染线程
	game_world->tick_SubSystem->BindEvent(renderer_center.ptr, "RenderSceneDataReady", *renderer_center->event_system.GetEventByName("OnRenderSceneDataReady"));
	game_world->tick_SubSystem->BindEvent(renderer_center.ptr, "RenderWidgetDataReady", *renderer_center->event_system.GetEventByName("OnRenderWidgetDataReady"));
	game_world->tick_SubSystem->BindEvent(renderer_center.ptr, "RenderClear", *renderer_center->event_system.GetEventByName("OnRenderClear"));
	game_world->tick_SubSystem->BindEvent(renderer_center.ptr, "RenderPresent", *renderer_center->event_system.GetEventByName("OnRenderPresent"));

	// game_world->tick_SubSystem->dispatcher_system.BindEventTo(renderer_center.ptr,"synchronization",Event("synchronization",[this](TEventParams e) {
	// 	this->renderer_center->ReadLeftCallback();
	// }));
}
std::shared_ptr<SDL_Texture> GameEngine::GetDefaultTexture()
{
	return renderer_center->DefaultTexture;
}




GCWeakPtr<Widget> AddToViewport(Widget* new_widget)
{
	return GameEngine::Instance().AddWidgetToViewport(new_widget);
}
GCWeakPtr<GameWorld> World()
{
	return GameEngine::Instance().GetGameWorld();
}

