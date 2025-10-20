#include "Classes/Core/TickManager.hpp"

#include "Classes/Core/GameEngine.hpp"
#include "Classes/Core/GameWorld.hpp"
#include "Classes/Controller.hpp"
#include "Types/RenderData.hpp"

void TickSubSystem::Tick(double delta_time)
{
    auto actors = GameEngine::Instance().GetGameWorld()->GetActors();
    std::vector<GCPtr<Controller>> controllers = GameEngine::Instance().GetGameWorld()->GetControllers();
    auto world = GameEngine::Instance().GetGameWorld();

	std::vector<RenderData> render_data;
	render_data.reserve(actors.size());
	if (buffer_type == 1)
	{
		//计算物理
		world->physicsSys.simulation(delta_time);
		//映射物理
		for (auto& a : actors)
		{
			if (a->IsActive())
			{
				a->PostPhysicsTick(delta_time);
			}
		}
		//普通Tick
		for (auto& a : actors)
		{
			if (a->IsActive())
			{
				a->Tick(delta_time);
				a->RootComponentTick(delta_time);
			}
		}
		//渲染纹理
		for (auto& a : actors)
		{
			if (a->is_pre_kill)
			{
				a->is_pending_kill = true;
				world->RemoveActorByGCPtr(a);
			}
			else if (a->IsActive())
			{
				a->RenderOnScreen();
			}
		}


		for (auto& controller : controllers)
		{
			controller->Tick(delta_time);
		}
	}
    else if (buffer_type == 2)//  双缓冲  ---------------------------------------------------------------------------------
    {
	    //PreTick

    	//普通Tick
    	for (auto& a : actors)
    	{
    		if (a->IsActive())
    		{
    			a->Tick(delta_time);
    			a->RootComponentTick(delta_time);
    		}
    	}
    	//提交渲染线程命令
    	for (auto& a : actors)
    	{
    		if (a->is_pre_kill)//
    		{
    			a->is_pending_kill = true;
    			world->RemoveActorByGCPtr(a);
    		}
    		else if (a->IsVisible())
    		{
    			a->ForRenderOrder(render_data);
    		}
    	}

    	for (auto& controller : controllers)
    	{
    		controller->Tick(delta_time);
    	}
    	EventParams render_data_ready_p;
    	render_data_ready_p.Add<std::vector<RenderData>>("render_data",std::move(render_data));

    	dispatcher_system.CallDispatcher("RenderDataReady", render_data_ready_p);


    	dispatcher_system.CallDispatcher("synchronization");


    }
}

void TickSubSystem::Init()
{
	NAME;
	dispatcher_system.AddEventDispatcher("RenderDataReady");
}


void TickSubSystem::SetBufferType(int type) { buffer_type = type; }
