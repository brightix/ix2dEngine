#pragma once
#include <condition_variable>
#include <thread>

#include "Classes/Actor.hpp"
#include "Classes/SubSystem/Sub/EngineSubsystem.hpp"
#include "Classes/Widget/ContentWidget/TextBlockWidget.hpp"
#include "SPhysics/SPhysics.hpp"


struct PhysicsBufferShot
{
    //只有拥有物理的actor
    std::vector<Actor*> actors;

};
class TickSubSystem : public EngineSubSystem
{
    std::vector<PhysicsBufferShot> physics_buffer;
    std::vector<std::atomic<bool>> fence;
    std::atomic<bool> is_stop;
    std::mutex mtx;
    std::condition_variable cv;
    int buffer_type;
    int write_head = 0;
    std::vector<GCPtr<TextBlockWidget>> texts;
public:
    TickSubSystem(int buffer_cnt);
	TickSubSystem() : buffer_type(0) { }

    void Construct() override;
	void RegisterDispatchers() override;
    void Tick(double delta_time);


	void Init() override;

	void SetBufferType(int type);
};