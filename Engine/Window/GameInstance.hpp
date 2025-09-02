#pragma once
#include <SDL3/SDL.h>
#include <FuncLib/ixStaticFuncLib.hpp>
#include <Actor.h>
#include <vector>
#include <memory>
#include <Timer.hpp>
#include <SystemConfig.hpp>
#include <iostream>
#include <Font.hpp>
class GameInstance
{
    SDL_Renderer* renderer;
    SDL_Window* window;
    std::vector<std::shared_ptr<Actor>> Actors;

    //系统数据
    SystemConfig SysConfig;

    double deltaTime;
    //std::vector<std::shared_ptr<UserWidget>> Actors;

    //Tick计时器
    Timer TickTimer;

private:
    GameInstance()
    {
        if (!SDL_Init(SDL_INIT_VIDEO)) 
        {
            LogToFile("SDL_Init Error",LogLevel::FatalError);
            return;
        }
        // 创建窗口
        window = SDL_CreateWindow(
            "Hello SDL3",                  // 标题
            800, 600,                      // 宽高
            SDL_WINDOW_RESIZABLE           // 可拉伸
        );
        if (!window) 
        {
            LogToFile("SDL_CreateWindow Error:" + std::string(SDL_GetError()));
            SDL_Quit();
            return;
        }

        // 创建渲染器
        renderer = SDL_CreateRenderer(window, NULL);
        if (!renderer) 
        {
            LogToFile("SDL_CreateRenderer Error: " + std::string(SDL_GetError()));
            SDL_DestroyWindow(window);
            SDL_Quit();
            return;
        }
        timeBeginPeriod(1);
        SysConfig = {60,{640,480}};
    }
public:
    static GameInstance& Instance()
    {
        static GameInstance instance;
        return instance;
    }
    void Tick()
    {
        bool running = true;
        while (running) {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_EVENT_QUIT) {
                    running = false;
                }
            }

            // 清屏（黑色）
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            // 在中间画一个红色矩形
            //场景Tick
            for(auto& a : Actors){
                a->Tick(deltaTime);
            }
            //std::cout << deltaTime << std::endl;
            //控件Tick
            
            // 显示到窗口
            SDL_RenderPresent(renderer);
            TickTimer.Delay((1.0/SysConfig.TargetFps) - TickTimer.End());
            deltaTime = TickTimer.End();
            TickTimer.Start();
        }
    }
    void Ready()
    {
        TickTimer.Start();
    }
    ~GameInstance()
    {    
        // 清理
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        timeEndPeriod(1);
    }
    void AddActor(std::shared_ptr<Actor> actor)
    {
        Actors.push_back(actor);
    }

    Vec2d<double> GetViewportSize() { return SysConfig.ViewportSize; }
    SDL_Renderer* GetRenderer() { return renderer; }
};