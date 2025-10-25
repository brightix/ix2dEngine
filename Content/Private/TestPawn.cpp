#include "public/TestPawn.hpp"

#include "Classes/Component/SenceComponent/StaticTexture.hpp"
#include "Classes/Core/GameEngine.hpp"
#include "Classes/Core/RendererCenter.hpp"
#include "Enum/ActorEnum.hpp"
#include "Utilities/FuncLib/Deleter.hpp"

TestPawn::TestPawn()
{
    name = "TestPawn";
}

void TestPawn::Construct()
{
	Pawn::Construct();
	mobility = ActorMobility::Movable;
	GetSceneComponent<StaticTexture>("default_texture")
		->SetNewTexture(TTexture(RendererCenter::CreateOutLineTexture({200,200},BLUE)));
}
