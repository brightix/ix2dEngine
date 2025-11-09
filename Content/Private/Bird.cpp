//
// Created by 21811 on 2025/11/9.
//

#include "../Public/Bird.hpp"

#include "../Public/TestGround.hpp"
#include "Classes/Component/SenceComponent/Capsule2d.hpp"

void Bird::EventBegin()
{
	Character::EventBegin();
	ListenDispatcher(capsule,"OnCollision",&Bird::OnCollision);
	capsule->GetPhysicsBody()->is_subscribe_collision = true;
	SetCharacterMoveStrategy(God);
}

void Bird::OnCollision(std::unordered_set<SPhysicsBaseUtility*> cols)
{
	for (auto& col : cols)
	{
		if (typeid(*col->collision_owner->outer) == typeid(TestGround))
		{
			std::cout << "游戏结束" << std::endl;
			CallDispatcher("GameOver");
			return;
		}
	}
}
