#include "Public/GroundManager.hpp"

#include "Classes/Controller.hpp"
#include "Classes/Pawn.hpp"
#include "Classes/Component/ActorComponent/MovableComponent.hpp"
#include "Classes/Core/GameWorld.hpp"
#include "Types/Enums/PivotDir.hpp"

void GroundManager::Construct()
{
	Actor::Construct();
	entrance = 300.f;
	Vec2<float> screen_size = Engine().GetEngineAttribution().ScreenSize;
	Engine().GetEngineSubSystemManager()->GetSubsystem<RandomUtility>()->RegisterRandom(name,{-screen_size.y * 0.4f, 0.f});
	Engine().GetEngineSubSystemManager()->GetSubsystem<RandomUtility>()->RegisterRandom("Entrance",{300.f, 600.f});
}

void GroundManager::RegisterDispatchers()
{
	Actor::RegisterDispatchers();
	AddDispatcher("OnAddScore");
}

void GroundManager::EventBegin()
{
	Actor::EventBegin();
}

void GroundManager::Tick(const double delta_time)
{
	Actor::Tick(delta_time);

	TubeMove(delta_time);
}

void GroundManager::TubeMove(double delta_time)
{
	if (!stop)
	{
		for (auto& [top,bottom] : grounds)
		{
			auto screen_size = Engine().GetEngineAttribution().ScreenSize;
			auto location = top->GetActorWorldLocation();
			auto tube_size = top->size;
			if (location.x < screen_size.x * 0.5f)
			{
				CallDispatcher("OnAddScore");
			}
			if (location.x + tube_size.x < 0)
			{
				//这一组如果出左界了
				float rand = Engine().GetEngineSubSystemManager()->GetSubsystem<RandomUtility>()->GetRandom(name).value();
				top->SetActorLocation({static_cast<float>(screen_size.x) + tube_size.x,rand});

				float entrance_rand = Engine().GetEngineSubSystemManager()->GetSubsystem<RandomUtility>()->GetRandom("Entrance").value();
				bottom->SetActorLocation({static_cast<float>(screen_size.x) + tube_size.x, tube_size.y + top->GetActorWorldLocation().y + entrance_rand});
				continue;
			}
			auto movement = Vec2<float>(delta_time * -150.f, 0);
			top->AddActorLocation(movement);
			bottom->AddActorLocation(movement);
		}
	}
}

void GroundManager::Init()
{
	for (auto& [top,bottom] : grounds)
	{
		top->DestroyActor();
		bottom->DestroyActor();
	}
	grounds.Clear();
	Vec2<float> screen_size = Engine().GetEngineAttribution().ScreenSize;
	Vec2<float> tube_size(100,screen_size.y);
	int n = 7;
	float interval = (screen_size.x - n * tube_size.x) / (n - 1);
	float start_x = screen_size.x;
	for (int i = 0; i < n - 1; ++i)
	{
		float rand = Engine().GetEngineSubSystemManager()->GetSubsystem<RandomUtility>()->GetRandom(name).value();
		float entrance_rand = Engine().GetEngineSubSystemManager()->GetSubsystem<RandomUtility>()->GetRandom("Entrance").value();

		auto top = SpawnActor<TestGround>();
		top->SetGroundSizeAndColor(tube_size, YELLOW);
		auto x_loc = start_x + interval * i * 2 + tube_size.x;
		top->SetActorLocation({x_loc, rand});
		top->GetGroundTexture()->SetComponentPivot(PivotDir::CENTER);

		auto bottom = SpawnActor<TestGround>();
		bottom->SetGroundSizeAndColor(tube_size, WHITE);
		bottom->SetActorLocation({x_loc, static_cast<float>(screen_size.y) + rand + entrance_rand});
		bottom->GetGroundTexture()->SetComponentPivot(PivotDir::CENTER);
		grounds.insert({top,bottom});
	}
	stop = false;
}

void GroundManager::Stop()
{
	stop = true;
}

void GroundManager::Select(int i)
{
	if (i == 1)
	{
		Init();
	}
	else if (i == 2)
	{
		Stop();
		Cast<MovableComponent>(World()->GetController()->GetControlledPawn()->GetActorComponent("MovableComponent"))->SetActiveMove(false);
	}
}


