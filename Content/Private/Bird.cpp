#include "../Public/Bird.hpp"

#include "../Public/TestGround.hpp"
#include "Classes/Component/ActorComponent/MovableComponent.hpp"
#include "Classes/Component/SenceComponent/Capsule2d.hpp"
#include "Classes/Component/SenceComponent/StaticTextureComponent.hpp"
#include "Classes/Core/GameEngine.hpp"
#include "Types/Enums/PivotDir.hpp"



void Bird::Construct()
{
	Character::Construct();
	CNAME;
	auto center = NewObject<StaticTextureComponent>();
	center->SetStaticTexture(Create_FilledTexture_S({3,3},GREEN));
	Root->MountedComponent<StaticTextureComponent>(center);
	Root->SetComponentPivot(PivotDir::BOTTOM_CENTER);
	//Root->GetSceneComponentByName("default_texture")->SetComponentPivot(PivotDir::CENTER);
	Cast<MovableComponent>(GetActorComponent("MovableComponent"))->SetMoveSpeed(500);
	capsule->GetPhysicsBody()->mass = 150.f;
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
	SetCharacterMoveStrategy(God);

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
