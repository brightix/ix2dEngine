#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <memory>

#include "Font.hpp"
#include "GameWorld.hpp"
#include "Texture.hpp"
#include "Classes/Actor.h"
#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
#include "Utilities/Timer.hpp"
#include "Structure/SystemConfig.hpp"
#include "Enum/LogLevel.h"
#include "Classes/Actor.h"
#include "Classes/GameModeBase.hpp"
#include "Utilities/ConverterRegistry.hpp"
#include "Classes/Widget/CanvasWidget.hpp"
#include "Utilities/ExternalWrapper.hpp"
#include "Utilities/GCPtr.hpp"

class GameEngine : public Object
{
    SDL_Renderer* renderer;
    SDL_Window* window;

    bool running;
    //系统数据
    SystemConfig SysConfig;

    double delta_time;
    //std::vector<std::shared_ptr<UserWidget>> Actors;

    //Tick计时器
    GCPtr<Timer> tick_timer;
    GCPtr<Timer> consume_timer;



    //类型转换
    ConverterRegistry reg;

	//组件
	GCPtr<GameWorld> game_world;
private:
    //只放全局变量初始化
    GameEngine();

public:
    static GameEngine& Instance()
    {
        static GameEngine instance;
        return instance;
    }
	void Tick();
    void Construct() override;
    void Quit()
    {
        running = false;
    }
    ~GameEngine();
//Get
    Vec2d<double> GetViewportSize() { return SysConfig.ViewportSize; }
    SDL_Renderer* GetRenderer() { return renderer; }
    GCPtr<GameWorld> GetGameWorld();
//Render
	void RenderTexture(GCPtr<Texture> texture, SDL_FRect location);
};
