#include "Utilities/TracingUtility.hpp"

nlohmann::json TracingUtility::rep = {
    { "CPU", GetCPUName() },
    { "MaxFps", std::numeric_limits<float>::lowest() },
    { "MinFps", std::numeric_limits<float>::max() },
    { "AverageFps", 0.f }
};

void TracingUtility::ReportPerformance(const EngineState &info)
{
    if (info.DeltaTime <= 0.0)
    {
        // 无效或未初始化的数据直接忽略
        return;
    }

    // 计算 FPS，防止极小值导致溢出
    float fps = 1.0f / std::max(info.DeltaTime, 1e-6f);

    // 屏蔽异常极端值（比如超高帧率或系统时间异常）
    if (fps <= 0.0f || fps > 10000.0f)
    {
        return;
    }
    // 使用 (std::max) 以避开 Windows 宏污染
    rep["MaxFps"] = (std::max)(rep["MaxFps"].get<float>(), fps);
    rep["MinFps"] = (std::min)(rep["MinFps"].get<float>(), fps);

    static int Total_Fps_Cnt = 0;
    float prevAvg = rep.value("AverageFps", 0.0f);
    ++Total_Fps_Cnt;
    float average = prevAvg + (fps - prevAvg) / static_cast<float>(Total_Fps_Cnt);
    rep["AverageFps"] = average;
}