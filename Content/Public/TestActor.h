#pragma once
#include <iostream>
#include "Classes/Actor.h"
#include "Types/EventParam.hpp"
#include "Utilities/EventDispatcherSystem.hpp"

class GameEngine;
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
        EventMethod e([](std::optional<EventParams> e	p) {
            std::cout << *ep->Get<string>("name") << std::endl;
        });
        AddCustomEvent("testMethod",e);
        CallEvent("testMethod",epp);
    }
    void Tick(double delta) override
    {
    	//GCPtr<ExternalWrapper<SDL_Texture>>(this,ExternalWrapper<SDL_Texture>(SDL_Texture({})))
    	GameEngine::Instance().RenderTexture(r);

    }
};
