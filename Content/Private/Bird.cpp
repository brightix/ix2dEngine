#include "../Public/Bird.hpp"

#include "../Public/TestGround.hpp"
#include "Classes/Controller.hpp"
#include "Classes/Component/ActorComponent/MovableComponent.hpp"
#include "Classes/Component/SenceComponent/Capsule2d.hpp"
#include "Classes/Component/SenceComponent/StaticTextureComponent.hpp"
#include "Classes/Core/GameEngine.hpp"
#include "Classes/Core/GameWorld.hpp"
#include "Types/Enums/PivotDir.hpp"



void Bird::Construct()
{
	Character::Construct();
	CNAME;
	Root->SetComponentPivot(PivotDir::BOTTOM_CENTER);
	//创建对象，添加到组件，设置属性
	auto center = NewObject<StaticTextureComponent>(this);
	AddSceneComponent(center);

	center->SetStaticTexture(Create_FilledTexture_S({3,3},GREEN));

	Cast<MovableComponent>(GetActorComponent("MovableComponent"))->SetMoveSpeed(500);
	capsule->GetPhysicsBody()->mass = 150.f;
	Init();
}

void Bird::RegisterDispatchers()
{
	Character::RegisterDispatchers();
	AddDispatcher("OnGameOver");
}

void Bird::EventBegin()
{
	Character::EventBegin();
	ListenDispatcher(capsule,"OnCollision",&Bird::OnCollision);
	capsule->GetPhysicsBody()->is_subscribe_collision = true;
	SetCharacterMoveStrategy(Simulation);
	Cast<MovableComponent>(GetActorComponent("MovableComponent"))->SetActiveMove(true);
}
void Bird::OnCollision(std::unordered_set<SPhysicsBaseUtility*> cols)
{
	if (start_game)
	{
		for (auto& col : cols)
		{
			if (typeid(*col->collision_owner->outer) == typeid(TestGround))
			{
				std::cout << "游戏结束" << std::endl;
				CallDispatcher("OnGameOver");
				start_game = false;
				return;
			}
		}
	}
}

void Bird::Init()
{
	auto screen_size = Engine().GetEngineAttribution().ScreenSize;
	SetActorLocation(screen_size/2);
	SetCharacterMoveStrategy(Simulation);
}
