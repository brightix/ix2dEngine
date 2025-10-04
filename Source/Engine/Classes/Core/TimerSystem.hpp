#pragma once
#include <functional>
#include <queue>
#include <chrono>
struct TimerTask
{
    std::chrono::time_point<std::chrono::steady_clock> end_time;
    std::function<int()> callback;
};

inline auto TimerTaskComp = [](TimerTask& a,TimerTask& b) {
    return a.end_time > b.end_time;
};

class TimerSystem
{
    std::priority_queue<TimerTask, std::vector<TimerTask>, decltype(TimerTaskComp)> task_queue;
public:
    TimerSystem();
    void Run();
    void SetTimer(int delay, std::function<int()> callback);
};

