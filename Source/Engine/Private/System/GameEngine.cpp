#include "System/GameEngine.hpp"

#include "System/Font.hpp"
#include "System/Texture.hpp"


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
	GCAllObjects.push_back(this);
	SysConfig = {120, {640, 480}};
	game_world = make_GCPtr<GameWorld>(new GameWorld());
	game_world->Construct();

	tick_timer = make_GCPtr<Timer>(new Timer());
	consume_timer = make_GCPtr<Timer>();
	running = true;
	game_world->StartSimulation();
	timer_system.SetTimer(2000,[this]() {
		printf("GCSweep!!\n");
		std::cout << "{" << GCSweep() << "} objects have been Swept!" << std::endl;
		return 2000;
	});
	auto dd = ConstructObjectFromClass(new Actor());
	timer_system.SetTimer(5000,[dd]() {
		dd->DestroyActor();
		return -1;
	});

}

void GameEngine::Tick()
{
	tick_timer->Start(); // 关键：第一次先 Start
	while (running) {
		consume_timer->Start();
		delta_time = tick_timer->End();   // 上一帧耗时
		tick_timer->Start();             // 重置计时

		// 清屏
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// 场景逻辑
		game_world->Tick(delta_time);
		timer_system.Run();


		// FPS 显示
		double fps = (delta_time > 0) ? (1.0 / delta_time) : 0.0;
		auto tex = FontRenderer::Instance().GetTextTexture("FPS: " + std::to_string(fps));
		SDL_FRect dst = {0,0,(float)tex->w,(float)tex->h};
		SDL_RenderTexture(renderer, tex.get(), nullptr, &dst);

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

void GameEngine::RenderTexture(GCPtr<StaticTexture> texture, SDL_FRect location)
{
	SDL_RenderTexture(renderer,texture.Get()->texture,nullptr,&location);
}

void GameEngine::GCMark(GCObject *gc_object)
{
	//对象不存在 or 已被标记
	if (!gc_object || gc_object->bMarked) return;
	gc_object->bMarked = true;
	for (auto child : gc_object->referencing)
	{
		GCMark(child);
	}
}

int GameEngine::GCSweep()
{
	for (auto obj : GCAllObjects)
	{
		obj->bMarked = false;
	}
	GCMark(this);
	int cnt = 0;
	for (GCObject* obj : GCAllObjects)
	{
		if (!obj->bMarked)
		{
			delete obj;
			cnt++;
		}
	}
	return cnt;
}