#pragma once
#include <Actor.h>

class TestActor : public Actor
{
public:
    int r;
    TestActor(int r) : r(r)
    {
    }
    void Tick(double delta) override 
    {
        SDL_FRect rect = { ((float)r)*100, 250, 100, 100 };
        SDL_SetRenderDrawColor(renderer, r*10, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
};