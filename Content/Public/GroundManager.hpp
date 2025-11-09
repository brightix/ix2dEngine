#pragma once
#include "TestGround.hpp"
#include "Classes/Actor.hpp"

class GroundManager : public Actor
{
	using Actor::Actor;
	Array<Vec2<TestGround*>> grounds;
public:
	void Construct() override;
	void EventBegin() override;
	void Tick(double delta_time) override;
};
