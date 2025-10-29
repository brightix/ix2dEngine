#include "public/TestPawn.hpp"

#include "Classes/Component/SenceComponent/CollisionBox.hpp"
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
	auto st = GetSceneComponent("default_texture").Cast<StaticTexture>();
	st->SetNewTexture(TTexture(Create_OutLineTexture_S({100,100})));
	collision_box = Root->MountedComponent(new CollisionBox());
	collision_box->SetBoundBox({st->w,st->h});
}
