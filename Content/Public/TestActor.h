#pragma once
#include <iostream>
#include "Classes/Actor.h"
#include "Types/EventParam.hpp"
#include "Utilities/EventDispatcherSystem.hpp"

using namespace std;
class TestActor : public Actor
{
public:
    int r;
    TestActor(int r) : r(r)
    {
        this->name = "ixActor";
        EventParams epp;
        epp.Add("name",name);
        EventMethod e([](std::optional<EventParams> ep) {
            std::cout << *ep->Get<string>("name") << std::endl;
        });
        AddCustomEvent("testMethod",e);
        CallEvent("testMethod",epp);
    }
    void Tick(double delta) override
    {
        SDL_FRect rect = { ((float)r)*100, 250, 100, 100 };
        SDL_SetRenderDrawColor(renderer, r*10, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);

    }
};
