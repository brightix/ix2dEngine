#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include "ThirdParty/json.hpp"
#include <fstream>

#include "FuncLib/ixStaticFuncLib.hpp"

struct TracingInfo
{
    std::string name;
    std::string ph;
    std::string pid;
    std::string tid;
    std::string ts;
};

class TracingUtility
{
    std::ofstream file;
    nlohmann::json begin;
    nlohmann::json end;
	std::string current_file_path;
	std::string to_write;
public:
    TracingUtility()
    {
    	//读配置
        const std::string path = "Content/Tracing/Config.json";
        std::ifstream in(path);
        if (!in.is_open()) {
            LogWithLevel("无法打开配置文件！", FatalError);
            return;
        }

        nlohmann::json j;
        in >> j;
        in.close();

        float version = j.value("TracingVersion", 0.0f);
        version += 0.001f;
        j["TracingVersion"] = version;

        // 写回配置文件
        std::ofstream out(path, std::ios::trunc);
        out << j.dump(4);
        out.close();


        // 生成带版本号的文件名
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(3) << version;
        std::string file_name = ss.str() + ".json";
    	current_file_path = "Content/Tracing/" + file_name;
        // file = std::ofstream(current_file_path);
        // if (!file.is_open()) {
        //     LogWithLevel("无法打开追踪文件！", FatalError);
        //     return;
        // }

        begin["ph"] = "B";              // 开始事件（Begin）
        begin["tid"] = "main";          // 线程名
        begin["cat"] = "function";      // 分类，可随意命名
        begin["pid"] = 0;               // 进程ID
        begin["args"] = nlohmann::json::object(); // 附加信息（可为空）
        end = begin;
        end["ph"] = "E";
        //file <<  "{\"traceEvents\": [" << std::endl;
    	to_write.reserve(1024*512);
    	std::cout << "开始记录tracing" << std::endl;
    }
    ~TracingUtility()
	{
    	// auto size = std::filesystem::file_size(current_file_path);
    	// std::filesystem::resize_file(current_file_path, size-1);
        // file << R"({}],"displayTimeUnit": "ms"})" << std::endl;
        // if (file.is_open()) {
        //     file.close();
        //     Log("tracing导出成功");
        // }
		std::ofstream file_t(current_file_path);
    	file_t <<  "{\"traceEvents\": [" << std::endl;
		file_t << to_write;
    	file_t << R"({}],"displayTimeUnit": "ms"})" << std::endl;
    	file_t.close();
    }
    static TracingUtility& Instance()
    {
        static TracingUtility instance;
        return instance;
    }
    void StartTracing(const std::string& tracing_name)
    {
        auto now = std::chrono::system_clock::now();

        // 转换为自纪元（1970-01-01）以来的毫秒数
        auto timestamp = std::chrono::duration_cast<std::chrono::microseconds>(
            now.time_since_epoch()
        ).count();

        nlohmann::json tracing = begin;
        tracing["name"] = tracing_name;
        tracing["ts"] = timestamp;
    	to_write.append(tracing.dump()+",");
        //file << tracing.dump(4) << "," << std::endl;
    }
    void EndTracing(const std::string& tracing_name)
    {
        auto now = std::chrono::system_clock::now();

        // 转换为自纪元（1970-01-01）以来的毫秒数
        auto timestamp = std::chrono::duration_cast<std::chrono::microseconds>(
            now.time_since_epoch()
        ).count();

        nlohmann::json tracing = end;
        tracing["name"] = tracing_name;
        tracing["ts"] = timestamp;\
    	to_write.append(tracing.dump()+",");
        //file << tracing.dump(4) << "," << std::endl;
    }
};