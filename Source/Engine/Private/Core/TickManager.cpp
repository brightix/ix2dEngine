#include "Classes/Core/TickManager.hpp"

#include "System/GameEngine.hpp"
#include "System/GameWorld.hpp"

void TickManager::Tick(double delta_time)
{
    std::vector<Actor*> actors = GameEngine::Instance().GetGameWorld()->GetActors();
    std::vector<GCPtr<Controller>> controllers = GameEngine::Instance().GetGameWorld()->GetControllers();
    auto world = GameEngine::Instance().GetGameWorld();
    switch (buffer_type)
    {
        case 1:
            //计算物理
            for (auto& a : actors)
            {
                if (a->IsActive())
                {
                    a->PrePhysicsTick(delta_time);
                }
            }
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
                    a->ActorComponentTick(delta_time);
                }
            }

            //渲染
        //碰撞箱
            for (auto& a : actors)
            {
                if (!a->is_pre_kill && a->IsActive())
                {
                    a->RenderCollisionBox();
                }
            }
        //渲染纹理
            for (auto& a : actors)
            {
                if (a->is_pre_kill)
                {
                    a->is_pending_kill = true;
                    world->RemoveActorByPtr(a);
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
            break;
        case 2:

    		//PreTick

    		//普通Tick
    		for (auto& a : actors)
    		{
    			if (a->IsActive())
    			{
    				a->Tick(delta_time);
    				a->ActorComponentTick(delta_time);
    			}
    		}

    		//Post
    		//计算物理
    		for (auto& a : actors)
    		{
    			if (a->IsActive())
    			{
    				a->PrePhysicsTick(delta_time);
    			}
    		}
    		//映射物理 使用四叉树碰撞检测
    		for (auto& a : actors)
    		{
    			if (a->IsActive())
    			{
    				a->PostPhysicsTick(delta_time);
    			}
    		}

    		//渲染
    		//碰撞箱
    		for (auto& a : actors)
    		{
    			if (!a->is_pre_kill && a->IsActive())
    			{
    				a->RenderCollisionBox();
    			}
    		}
    		//渲染纹理
    		for (auto& a : actors)
    		{
    			if (a->is_pre_kill)//
    			{
    				a->is_pending_kill = true;
    				world->RemoveActorByPtr(a);
    			}
    			else if (a->IsVisible())
    			{
    				//将任务交给渲染线程
    				//a->RenderOnScreen();

    			}
    		}


    		for (auto& controller : controllers)
    		{
    			controller->Tick(delta_time);
    		}
            break;
        case 3:
            break;
        default: break;
    }
}

TickManager::TickManager() : buffer_type(1) {}

void TickManager::CreateWorker(std::function<void()> task)
{
    thread.emplace_back(task);
}

void TickManager::PhysicsBuffer(std::vector<Actor*> actors)
{
    int current = write_head % 3;
    RenderBufferShot buffer_shot;
    for (int i = 0; i < actors.size(); i++)
    {
        auto& actor = actors[i];
        buffer_shot.actors.emplace_back(actor);
        buffer_shot.transforms.emplace_back(actor->GetWorldTransform());
    }
        fence[current].store(true,std::memory_order_release);
}

void TickManager::Render()
{
    int current = (write_head+2) % 3;

    // if (fence[current].load(std::memory_order_acquire))
    // {
    //     //auto& buffer_shot = render_buffer[current];
    //     int n =  buffer_shot.actors.size();
    //     for (int i = 0; i < n; i++)
    //     {
    //         auto& actor = buffer_shot.actors[i];
    //         if (actor->is_pre_kill)
    //         {
    //             actor->is_pending_kill = true;
    //         }
    //         else
    //         {
    //             actor->RenderOnScreen();
    //         }
    //     }
    // }
    fence[current].store(false);
}
