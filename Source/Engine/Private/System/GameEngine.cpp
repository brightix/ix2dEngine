#include "../../Classes/Core/GameEngine.hpp"

#include "System/Font.hpp"
#include "../../Classes/Component/SenceComponent/Texture.hpp"
#include "Classes/Core/GameWorld.hpp"
#include "Classes/SubSystem/Sub/SubsystemManager.hpp"

GameEngine::GameEngine() : delta_time(0)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		LogWithLevel("SDL_Init Error", LogLevel::FatalError);
		return;
	}
	// 创建窗口
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

	// 创建渲染器
	renderer = SDL_CreateRenderer(window, nullptr);
	if (!renderer)
	{
		Log("SDL_CreateRenderer Error: " + std::string(SDL_GetError()));
		SDL_DestroyWindow(window);
		SDL_Quit();
		return;
	}
	timeBeginPeriod(1);
}

void GameEngine::Construct()
{
	//Object::Construct();
	//将自己添加进全局GC
	GCAllObjects.emplace_back(this);
	GCRoot = this;
	SysConfig = {120, {640, 480}};
	game_world = make_GCPtr<GameWorld>(new GameWorld());
	game_world->Construct();

	tick_timer = make_GCPtr<NewTimer>(new NewTimer());
	consume_timer = make_GCPtr<NewTimer>();
	running = true;
	game_world->StartSimulation();
	//临时测试 垃圾回收 系统
	// timer_system.SetTimer(2000,[this]() {
	// 	std::cout << "{ " << GCSweep() << " } objects have been Swept!" << std::endl;
	// 	return 2000;
	// });
	engine_subsystem = NewObject<SubsystemManager<EngineSubSystem>>(new SubsystemManager<EngineSubSystem>());

	engine_subsystem->CreateSubSystem<GarbageCollection>("GarbageCollection");
	GarbageCollection* gc =  dynamic_cast<GarbageCollection*>(engine_subsystem->GetSubSystem("GarbageCollection"));
	timer_system.SetTimer(2000,[gc]() {
		gc->GCSweep();
		// std::cout << "{ " <<  << " } objects have been Swept!" << std::endl;
		return 2000;
	});
}

void GameEngine::Tick()
{
	tick_timer->Start(); // 关键：第一次先 Start

	auto* fpsTex = FontRenderer::Instance().GetTextTexture("            ");
	auto tex = StaticTexture({500,100});
	//FontRenderer::Instance().UpdateTextTexture(&tex, "test");
	//SDL_FRect dst = {0,0,(float)tex.w,(float)tex.h};

	SDL_FRect dst = SDL_FRect(0,0,fpsTex->w,fpsTex->h);
	while (running) {
		consume_timer->Start();
		delta_time = tick_timer->Click();       // 重置计时
		//delta_time = tick_timer->Click();
		//printf("%f\n",delta_time);
		// 清屏
		SDL_SetRenderDrawColor(renderer, 100, 100, 100,0);
		SDL_RenderClear(renderer);

		// 场景逻辑
		game_world->Tick(delta_time);
		timer_system.Run();

		double fps = 1.0 / delta_time;
		FontRenderer::Instance().UpdateTextTexture(fpsTex->GetTexture(), std::to_string(fps));

		// FPS 显示
		SDL_RenderTexture(renderer, fpsTex->GetTexture(), nullptr, &dst);

		// 显示到窗口
		SDL_RenderPresent(renderer);

		// 控制帧率
		tick_timer->Delay((1.0 / SysConfig.TargetFps) - consume_timer->End());
	}
}



void GameEngine::EventBegin()
{

}

GameEngine::~GameEngine()
{
	// 清理
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	timeEndPeriod(1);
}

GCPtr<GameWorld> GameEngine::GetGameWorld()
{
	return game_world;
}

GCObject *GameEngine::GetGCRoot() const
{
	return GCRoot;
}

void GameEngine::RenderTexture(GCPtr<StaticTexture> texture, SDL_FRect location)
{
	SDL_RenderTexture(renderer,texture.Get()->texture,nullptr,&location);
}
