#include "Window/GameEngine.hpp"


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
		800, 600,            // 宽高
		SDL_WINDOW_RESIZABLE // 可拉伸
	);
	if (!window)
	{
		Log("SDL_CreateWindow Error:" + std::string(SDL_GetError()));
		SDL_Quit();
		return;
	}

	// 创建渲染器
	renderer = SDL_CreateRenderer(window, NULL);
	if (!renderer)
	{
		Log("SDL_CreateRenderer Error: " + std::string(SDL_GetError()));
		SDL_DestroyWindow(window);
		SDL_Quit();
		return;
	}
	timeBeginPeriod(1);
	tick_timer = make_GCPtr<Timer>(this);
	consume_timer = make_GCPtr<Timer>(this);
	SysConfig = {120, {640, 480}};
	game_world = make_GCPtr<GameWorld>(this);
	running = true;
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
		// FPS 显示
		double fps = (delta_time > 0) ? (1.0 / delta_time) : 0.0;
		auto tex = FontRenderer::Instance().GetTextTexture("FPS: " + std::to_string(fps));
		SDL_FRect dst = {0,0,(float)tex->w,(float)tex->h};
		SDL_RenderTexture(renderer, tex.get(), nullptr, &dst);

		// 显示到窗口
		SDL_RenderPresent(renderer);

		// 控制帧率
		//double frameDuration = (1.0 / SysConfig.TargetFps) - deltaTime;
		tick_timer->Delay((1.0 / SysConfig.TargetFps) - consume_timer->End());
	}
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

void GameEngine::RenderTexture(float r)
{
	//GCPtr<ExternalWrapper<SDL_Texture>> texture,
	//SDL
	SDL_FRect rect = { r*100, 250, 100, 100 };
	SDL_SetRenderDrawColor(renderer, r*10, 0, 0, 255);
	SDL_RenderFillRect(renderer, &rect);
}


