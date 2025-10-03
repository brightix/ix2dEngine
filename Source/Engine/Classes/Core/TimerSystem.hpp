#pragma once
#include <functional>
#include <queue>

#include <windows.h>
struct TimerTask
{
    int delay_ms;
    LARGE_INTEGER start_time;
    std::function<int()> callback;
};

inline auto TimerTaskComp = [](TimerTask& a,TimerTask& b) {
    return a.delay_ms > b.delay_ms;
};

class TimerSystem
{
    LARGE_INTEGER freq;
    std::priority_queue<TimerTask, std::vector<TimerTask>, decltype(TimerTaskComp)> task_queue;
public:
    TimerSystem();
    void Run();
    void SetTimer(int delay, std::function<int()> callback);
};

