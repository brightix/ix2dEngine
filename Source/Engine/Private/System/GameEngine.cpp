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

	tick_timer = make_GCPtr<NewTimer>(new NewTimer());
	consume_timer = make_GCPtr<NewTimer>();
	running = true;
	game_world->StartSimulation();
	//临时测试 垃圾回收 系统
	// timer_system.SetTimer(2000,[this]() {
	// 	std::cout << "{ " << GCSweep() << " } objects have been Swept!" << std::endl;
	// 	return 2000;
	// });
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

void GameEngine::RenderTexture(GCPtr<StaticTexture> texture, SDL_FRect location)
{
	SDL_RenderTexture(renderer,texture.Get()->texture,nullptr,&location);
}

void GameEngine::GCMark(GCObject *gc_object)
{
	//对象不存在 or 已被标记
	if (!gc_object || gc_object->bMarked || gc_object->is_pending_kill) return;
	gc_object->bMarked = true;
	for (auto child : gc_object->referencing)
	{
		GCMark(child);
	}
}

int GameEngine::GCSweep()
{
	for (auto& obj : GCAllObjects)
	{
		obj->bMarked = false;
	}
	GCMark(this);
	std::vector<GCObject *> temp;
	int size = GCAllObjects.size();
	//temp.reserve(size);
	int cnt = 0;

	std::vector<GCObject*> to_delete;
	to_delete.reserve(GCAllObjects.size());

	for (auto& obj : GCAllObjects)
	{
		if (!obj->bMarked)
		{
			to_delete.emplace_back(obj);
			cnt++;
		}
		else temp.emplace_back(obj);
	}
	for (int i = 0; i < to_delete.size(); i++)
	{
		delete to_delete[i];
	}

	GCAllObjects.swap(temp);
	return cnt;
}