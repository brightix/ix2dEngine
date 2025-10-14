#include "Classes/Core/TimerSystem.hpp"

#include <iostream>

using TPS = std::chrono::time_point<std::chrono::steady_clock>;
using namespace std::chrono;
TimerSystem::TimerSystem()
{
}
void TimerSystem::Run()
{
    const TPS now = steady_clock::now();
    while (!task_queue.empty() && now > task_queue.top().end_time)
    {
        const auto&[end_time,callback] = task_queue.top();
        if(const int nxt_time = task_queue.top().callback();nxt_time > 0)
        {
            SetTimer(nxt_time, callback);
        }
        task_queue.pop();
    }
}

void TimerSystem::SetTimer(int delay, std::function<int()> callback)
{
    TPS now = steady_clock::now();
    task_queue.emplace(now + milliseconds(delay),std::move(callback));
}

