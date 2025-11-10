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
	Engine().GetEngineSubSystemManager()->GetSubsystem<RandomUtility>()->RegisterRandom(name,{-screen_size.y * 0.5f, -screen_size.y * 0.1f});
	Engine().GetEngineSubSystemManager()->GetSubsystem<RandomUtility>()->RegisterRandom("Entrance",{300.f, 500.f});
}

void GroundManager::RegisterDispatchers()
{
	Actor::RegisterDispatchers();
	AddDispatcher("OnAddScore",{TypeID(int)});
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
		for (int i = 0; i < grounds.size(); ++i)
		{
			auto& [top,bottom] = grounds[i];
			auto screen_size = Engine().GetEngineAttribution().ScreenSize;
			auto location = top->GetActorWorldLocation();
			auto tube_size = top->size;
			if (location.x < screen_size.x * 0.5f && !is_used[i])
			{
				CallDispatcher("OnAddScore",++score);
				is_used[i] = true;
			}
			if (location.x + tube_size.x / 2 < 0)
			{
				//这一组如果出左界了
				float rand = Engine().GetEngineSubSystemManager()->GetSubsystem<RandomUtility>()->GetRandom(name).value();
				top->SetActorLocation({static_cast<float>(screen_size.x) + tube_size.x / 2,rand});

				float entrance_rand = Engine().GetEngineSubSystemManager()->GetSubsystem<RandomUtility>()->GetRandom("Entrance").value();
				bottom->SetActorLocation({static_cast<float>(screen_size.x) + tube_size.x/2, tube_size.y + top->GetActorWorldLocation().y + entrance_rand});
				is_used[i] = false;
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
	int n = 6;
	float interval = (screen_size.x - n * tube_size.x) / (n - 1);
	float start_x = screen_size.x;
	is_used = std::vector(n,false);
	for (int i = 0; i < n; ++i)
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
	score = 0;
	CallDispatcher("OnAddScore",score);
	stop = false;
}

void GroundManager::Stop()
{
	stop = true;
}