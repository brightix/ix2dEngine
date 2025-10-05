#pragma once
#include <condition_variable>
#include <thread>

#include "Classes/Actor.hpp"


struct RenderBufferShot
{
    std::vector<Actor*> actors;
    std::vector<Transform> transforms;
    std::vector<GCPtr<StaticTexture>> sprite;
};

struct PhysicsBufferShot
{
    //只有拥有物理的actor
    std::vector<Actor*> actors;

};
class RenderBufferSystem : public Object
{
    std::vector<RenderBufferShot> render_buffer;
    std::vector<PhysicsBufferShot> physics_buffer;
    std::vector<std::thread> thread;
    std::vector<std::atomic<bool>> fence;
    std::atomic<bool> is_stop;
    std::mutex mtx;
    std::condition_variable cv;

    int buffer_type;
    int write_head = 0;
public:
    RenderBufferSystem(int buffer_cnt) : buffer_type(buffer_cnt), fence(buffer_cnt)
    {
        render_buffer.resize(buffer_cnt);
        CreateWorker([this] {
            while (!is_stop)
            {

            }
            Render();
        });
    }
    void Tick(double delta_time);
    RenderBufferSystem();
    void CreateWorker(std::function<void()> task);
    void PhysicsBuffer(std::vector<Actor *> actors);
    void Render();
};