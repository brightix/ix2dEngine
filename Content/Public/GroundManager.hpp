#pragma once
#include "TestGround.hpp"
#include "Classes/Actor.hpp"

class GroundManager : public Actor
{
	using Actor::Actor;
	Array<Vec2<TestGround*>> grounds;
	float entrance;
	bool stop;

public:
	void Construct() override;
	void RegisterDispatchers() override;
	void EventBegin() override;
	void Tick(double delta_time) override;
	void TubeMove(double delta_time);

	void Init();

	void Stop();

	void Select(int i);
};
