#pragma once
#include <condition_variable>
#include <thread>

#include "Classes/Actor.hpp"
#include "Classes/SubSystem/Sub/EngineSubsystem.hpp"
#include "SPhysics/SPhysics.hpp"


struct PhysicsBufferShot
{
    //只有拥有物理的actor
    std::vector<Actor*> actors;

};
class TickSubSystem : public EngineSubSystem
{
    std::vector<PhysicsBufferShot> physics_buffer;
	std::thread render_thread;
    std::vector<std::atomic<bool>> fence;
    std::atomic<bool> is_stop;
    std::mutex mtx;
    std::condition_variable cv;
    int buffer_type;
    int write_head = 0;
public:
    TickSubSystem(int buffer_cnt) : buffer_type(buffer_cnt), fence(buffer_cnt){}
	TickSubSystem() : buffer_type(0) { }

    void Tick(double delta_time);


	void Init() override;

	void SetBufferType(int type);
};