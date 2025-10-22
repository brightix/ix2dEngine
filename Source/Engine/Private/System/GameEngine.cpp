#include "../../Classes/Core/GameEngine.hpp"

#include "System/Font.hpp"
#include "../../Classes/Component/SenceComponent/Texture.hpp"
#include "Classes/Component/SenceComponent/StaticTexture.hpp"
#include "Classes/Core/GameWorld.hpp"
#include "Classes/SubSystem/Sub/SubsystemManager.hpp"
#include "Types/FontStyle.hpp"
#include "Types/RenderData.hpp"
#include "Utilities/FuncLib/Deleter.hpp"

GameEngine::GameEngine() : delta_time(0)
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
	GCRoot = this;
	SysConfig = {120, {640, 480}};

	game_world = make_GCPtr<GameWorld>(new GameWorld());
	game_world->Construct();

	tick_timer = make_GCPtr<NewTimer>(new NewTimer());
	consume_timer = make_GCPtr<NewTimer>();

	engine_subsystem->CreateSubSystem<GarbageCollection>("GarbageCollection");
	texture_store = engine_subsystem->CreateSubSystem<TextureStoreSubSystem>("TextureStoreSubSystem");


	//thread_sub_system->dispatcher_system.BindEventTo("RenderDataReady",);
	// game_world->tick_manager.dispatcher_system.BindEventTo("RenderDataReady",game_world.Get(),Event("HandleRenderDataReady",[](TEventParams e) {
	//
	// }));
}


void GameEngine::EventBegin()
{
	GCWeakPtr<GarbageCollection> gc = engine_subsystem->GetSubSystem<GarbageCollection>("GarbageCollection");
	// timer_system.SetTimer(2000,[gc]() {
	// 	if (auto p = gc.Peek())
	// 	{
	// 		p->GCSweep();
	// 	}
	// 	// std::cout << "{ " <<  << " } objects have been Swept!" << std::endl;
	// 	return 2000;
	// });
	//tick管理器 绑定到 渲染线程
	game_world->tick_SubSystem->dispatcher_system.BindEventTo(renderer_center.ptr, "RenderDataReady", *renderer_center->event_system.GetEventByName("HandleRenderDataReady"));
	game_world->tick_SubSystem->dispatcher_system.BindEventTo(renderer_center.ptr, "WidgetDataReady", *renderer_center->event_system.GetEventByName("HandleWidgetDataReady"));

	game_world->tick_SubSystem->dispatcher_system.BindEventTo(renderer_center.ptr,"synchronization",Event("synchronization",[this](TEventParams e) {
		this->renderer_center->ReadLeftCallback();
	}));
	//
	game_world->StartSimulation();
}


void GameEngine::Tick()
{
	tick_timer->Start(); // 关键：第一次先 Start

	auto fps_surface = font_manager->GetTextSurface("            ",{});
	auto fpsTex = NewObject(new StaticTexture());
	RendererCenter::SetTextureFromSurface(fpsTex.Get(),fps_surface);
	//FontRenderer::Instance().UpdateTextTexture(&tex, "test");
	//SDL_FRect dst = {0,0,(float)tex.w,(float)tex.h};

	SDL_FRect dst = SDL_FRect(0,0,fpsTex->w,fpsTex->h);
	while (running) {
		consume_timer->Start();
		delta_time = tick_timer->Click();       // 重置计时
		//delta_time = tick_timer->Click();
		//printf("%f\n",delta_time);


		// 场景逻辑
		game_world->Tick(delta_time);
		timer_system.Run();

		double fps = 1.0 / delta_time;
		FontRenderer::Instance().UpdateTextTexture(fpsTex->GetTexture().get(), std::to_string(fps));

		// FPS 显示

		 // RenderTask t;
		 // t.task = []() {
		 // 	EventParams e;
		 // 	e.Add("new_texture",);
		 // 	return e;
		 // };
		//UMG
		// RendererCenter::Render(widgets);
		// RendererCenter::AddRendererTask(RenderTask([fpsTex,dst](SDL_Renderer* renderer) {
		// 	SDL_RenderTexture(renderer, fpsTex->GetTexture().get(), nullptr, &dst);
		// }));

		//主线程查看回调函数 通知任务完成
		renderer_center->ReadLeftCallback();

		// 控制帧率
		tick_timer->Delay((1.0 / SysConfig.TargetFps) - consume_timer->End());
	}
	game_world->WorldDestroy();
}


GameEngine::~GameEngine()
{
	// 清理
	engine_subsystem->DeInitAllSubSystem();
	timeEndPeriod(1);
}

GCPtr<GameWorld> GameEngine::GetGameWorld()
{
	return game_world;
}

GCWeakPtr<Widget> GameEngine::AddWidgetToViewport(Widget *widget)
{
	auto ret = NewObject(widget);
	widgets.emplace(ret);
	return GCWeakPtr<Widget>(ret);
}

EngineState GameEngine::GetEngineAttribution() const
{
	EngineState engine_state;
	engine_state.DeltaTime = delta_time;
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



GCWeakPtr<Widget> AddToViewport(Widget* new_widget)
{
	return GameEngine::Instance().AddWidgetToViewport(new_widget);
}



