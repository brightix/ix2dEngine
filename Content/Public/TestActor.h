#pragma once
#include <iostream>
#include "Classes/Actor.hpp"
#include "../../Source/Engine/Classes/Component/SenceComponent/Texture.hpp"

class GameEngine;
using namespace std;
class TestActor : public Actor
{
public:
    int r;

	GCPtr<StaticTexture> texture;

	SDL_FRect rect;

    explicit TestActor(int r) : r(r), rect() {}

	void Construct() override
    {
    	this->name = "ixActor";
    	EventParams epp;
    	epp.Add("name",name);
    	EventMethod e([](std::optional<EventParams> ep) {
			std::cout << *ep->Get<string>("name") << std::endl;
		});
    	AddCustomEvent("testMethod",std::move(e));
    	CallEvent("testMethod",epp);
    	texture = NewObject(new StaticTexture({120,120},{255,255,255,255}));

    	rect = { 100, 250, 100, 100 };
    }
    void Tick(double delta) override
    {
    	//GameEngine::Instance().RenderTexture(texture,rect);
    }
};
