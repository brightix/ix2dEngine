#include "Public/GroundManager.hpp"

#include "Classes/Core/GameWorld.hpp"
#include "Types/Enums/PivotDir.hpp"

void GroundManager::Construct()
{
	Actor::Construct();
	Vec2<float> screen_size = Engine().GetEngineAttribution().ScreenSize;
	Engine().GetEngineSubSystemManager()->GetSubsystem<RandomUtility>()->RegisterRandom(name,{screen_size.y * 0.333f, screen_size.x * 0.666f});
	// for (int i = 0; i < 4 ; ++i)
	// {
	// 	float interval = 300.f;
	// 	auto top = SpawnActor<TestGround>();
	// 	top->SetGroundSize({100,static_cast<float>(screen_size.y)});
	// 	top->SetActorLocation({interval * i, screen_size.y /2.f});
	// 	top->GetGroundTexture()->SetComponentPivot(PivotDir::CENTER);
	// 	//->SetGroundSize(Vec2<float>(1200, 200));
	// 	auto bottom = SpawnActor<TestGround>();
	// 	bottom->SetGroundSize({100,static_cast<float>(screen_size.y)});
	// 	bottom->SetActorLocation({interval * i, screen_size.y});
	// 	bottom->GetGroundTexture()->SetComponentPivot(PivotDir::CENTER);
	// 	grounds.insert({top,bottom});
	// }
	auto t = SpawnActor<TestGround>();
	t->SetGroundSize({100, (screen_size.y)});
	t->SetActorLocation({0, 0});
	t->GetGroundTexture()->SetComponentPivot(PivotDir::CENTER);
}

void GroundManager::EventBegin()
{
	Actor::EventBegin();

}

void GroundManager::Tick(const double delta_time)
{
	Actor::Tick(delta_time);
	// for (auto& [top,bottom] : grounds)
	// {
	// 	auto location = top->GetActorWorldLocation();
	// 	auto size = top->size;
	// 	if (location.x + size.x < 0)
	// 	{
	// 		//这一组如果出左界了
	// 		auto screen_size = Engine().GetEngineAttribution().ScreenSize;
	// 		float new_y = Engine().GetEngineSubSystemManager()->GetSubsystem<RandomUtility>()->GetRandom(name).value();
	//
	// 		top->SetActorLocation({static_cast<float>(screen_size.x),new_y});
	// 		bottom->SetActorLocation({static_cast<float>(screen_size.x),new_y + 200});
	// 		continue;
	// 	}
	// 	top->AddActorLocation({-1,0});
	// }
}
