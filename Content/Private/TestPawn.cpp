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
	RenderTask task;
	auto tex = GetDefaultTexture();
	task.task = []  {
		// auto s = RendererCenter::CreateOutLineTexture({0,0,500,500}).get();
		// tex.reset(s);
		EventParams e;
		e.Add("new_texture",RendererCenter::CreateOutLineTexture({0,0,500,500}));
		return e;
	};
	task.callback = [this](EventParams e) {
		GetSceneComponent<StaticTexture>("default_texture")->SetStaticTexture(*e.Get<std::shared_ptr<SDL_Texture>>("new_texture"));
	};
	task.task_name = "修改test pawn的贴图";
	NewRendererTask(task);
}
