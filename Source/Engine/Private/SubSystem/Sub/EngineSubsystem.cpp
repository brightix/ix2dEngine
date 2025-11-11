#include "Classes/SubSystem/Sub/EngineSubsystem.hpp"

#include "Classes/Core/GameEngine.hpp"

void EngineSubSystem::Init()
{
	SubsystemBase::Init();
	outer = &Engine();
}
