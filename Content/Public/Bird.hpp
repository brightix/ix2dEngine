#pragma once
#include <unordered_set>

#include "Classes/Character.hpp"

class Bird : public Character
{
	using Character::Character;
public:
	void EventBegin() override;

	void OnCollision(std::unordered_set<SPhysicsBaseUtility*> cols);

};