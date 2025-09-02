#pragma once
#include <windows.h>
class Timer
{
    LARGE_INTEGER freq, start, end;
public:
    Timer()
    {
        QueryPerformanceFrequency(&freq);
    }
    void Start()
    {
        QueryPerformanceCounter(&start);
    }

    double End()
    {
        QueryPerformanceCounter(&end);
        return static_cast<double>(end.QuadPart - start.QuadPart) / freq.QuadPart;
    }
     
    void Delay(double duration)
    {
        LARGE_INTEGER begin,now;
        QueryPerformanceCounter(&begin);

        double elapsed = 0.0;

        // 先用 Sleep 粗略等待大部分时间
        if (duration > 0.005) // 如果延迟大于 5ms
        {
            DWORD sleepTime = static_cast<DWORD>((duration - 0.002) * 1000.0);
            Sleep(sleepTime);
        }
        // 精确等待剩余时间（忙等）
        do {
            QueryPerformanceCounter(&now);
            elapsed = static_cast<double>(now.QuadPart - begin.QuadPart) / freq.QuadPart;
        } while (elapsed < duration);
    }
};