#pragma once
#include <windows.h>
#include "Classes/Object.hpp"
#include <chrono>
#include <thread>

#include "TracingUtility.hpp"

using namespace std::chrono;

class Timer : public Object
{
    LARGE_INTEGER freq, start, end, last;
	double target_time;
    bool has_task;
public:
    Timer() : start(), end()
    {
	    last = {};
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

    //每次调用返回距上次调用的时间
    double Click()
    {
        LARGE_INTEGER now;
        double time = 0.0;
        if(last.QuadPart != 0){
            QueryPerformanceCounter(&now);
            time = static_cast<double>(now.QuadPart - last.QuadPart) / freq.QuadPart;
        }
        QueryPerformanceCounter(&last);
        return time;
    }

	void SetTimer(double delay_time,std::function<void()> callback_func)
    {
        has_task = true;

    }
};


class NewTimer : public Object
{
    time_point<steady_clock> last;
public:
    NewTimer()
    {
        last = {};
    	NAME;
    }

    void Start()
    {
        last = steady_clock::now();
    }

    double End() const
    {
        time_point<steady_clock> now = steady_clock::now();
        return duration<double>(now-last).count();
    }
//秒
    void Delay(double duration_s)
    {
    	//std::cout << "需要delay    " << duration_s << std::endl;
        TStart;
        auto start = steady_clock::now();
        auto end = start + duration<double>(duration_s);

        double coarse_ms = duration<double>(duration_s).count() - 0.008;
        // 粗略等待（睡掉绝大部分时间）
        if (coarse_ms > 0.0)
            std::this_thread::sleep_for(duration<double>(coarse_ms));

        // 精确等待剩余部分（忙等）
        while (steady_clock::now() < end) {
            _mm_pause();
        }
        TEnd;
    }

    //每次调用返回距上次调用的时间
    double Click()
    {
        const time_point<steady_clock> now = steady_clock::now();
        if(last == time_point<steady_clock>{}){
            last = now;
            return 0.0;
        }
        const double ret = duration<double>(now-last).count();
        last = now;
        return ret;
    }
};