#pragma once
#include <iostream>
#include "Classes/Actor.h"
#include "System/Texture.hpp"
#include "Types/EventParam.hpp"
#include "Utilities/EventDispatcherSystem.hpp"

class GameEngine;
using namespace std;
class TestActor : public Actor
{
public:
    int r;

	GCPtr<Texture> texture;

	SDL_FRect rect;
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
    	texture = SpawnActorFromSelf(new Texture({120,120},{255,255,255,255}));

    	rect = { 100, 250, 100, 100 };
    }
    void Tick(double delta) override
    {
    	GameEngine::Instance().RenderTexture(texture,rect);
    }
};
