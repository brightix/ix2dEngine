#include "Classes/Core/TimerSystem.hpp"

#include <iostream>

TimerSystem::TimerSystem()
{
    QueryPerformanceFrequency(&freq);
}
void TimerSystem::Run()
{
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    while (!task_queue.empty())
    {
        const auto&[delay_ms, start_time, callback] = task_queue.top();
        auto dd = static_cast<double>(now.QuadPart - start_time.QuadPart) / freq.QuadPart;
        if (dd*1000.0 > delay_ms)
        //if (now.QuadPart - start_time.QuadPart > static_cast<__int64>(delay_ms) * freq.QuadPart)
        {
            if(const int nxt_time = task_queue.top().callback();nxt_time > 0)
            {
                SetTimer(nxt_time, callback);
            }
            task_queue.pop();
        }
        else
        {
            break;
        }
    }
}

void TimerSystem::SetTimer(int delay, std::function<int()> callback)
{
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    task_queue.emplace(delay,now,std::move(callback));
}

