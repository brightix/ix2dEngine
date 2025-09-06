#include "Classes/Controller.hpp"
#include <SDL3/SDL.h>
Controller::Controller()
{
    controlled_pawn;
}

void Controller::Tick(double delta)
{
    Actor::Tick(delta);
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        // 根据事件类型分发
        event.type;
    }

}
