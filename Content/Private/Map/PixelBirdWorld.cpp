#include "Public/Map/PixelBirdWorld.hpp"

#include "Public/GroundManager.hpp"
#include "Public/TestGround.hpp"

PixelBirdWorld::PixelBirdWorld()
{
    CNAME;
}


void PixelBirdWorld::ConstructWorld()
{
	GameWorld::ConstructWorld();
	SpawnActor<GroundManager>();
}

void PixelBirdWorld::NativeWorldTick(const double delta_time)
{
	GameWorld::NativeWorldTick(delta_time);
}


