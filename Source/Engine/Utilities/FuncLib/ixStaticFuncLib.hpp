#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include "Utilities/magic_enum.hpp"
#include "Enum/LogLevel.h"
#include "Utilities/json.hpp"

#define Log(msg) LogToFile(msg, std::string(__FILE__), std::to_string(__LINE__), std::string(__func__))
#define LogWithLevel(msg,level) LogToFile(msg, std::string(__FILE__), std::to_string(__LINE__), std::string(__func__), level)

inline void LogToFile(const std::string msg, std::string file_name = "", std::string line = "", std::string func_name = "", LogLevel level = LogLevel::Tip)
{
    std::ofstream file("Log.txt",std::ios::app);
    if(!file.is_open())
    {
        std::cerr << "文件打开失败" << std::endl;
    }
    // 转换为 time_t （方便格式化输出）
    std::time_t now_c = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm tm = *std::localtime(&now_c);

    std::ostringstream timeStream;
    timeStream << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    std::string logText = timeStream.str() + file_name + line + func_name;

	//时间 + 文件路径 + 行数 + 问题严重程度 + 问题描述
    logText.append(" [" + std::string(magic_enum::enum_name(level)) + "]: " + msg);
    file << logText << std::endl;
    file.close();
    if (level == FatalError)
    {
        std::cerr << "致命错误" << std::endl;
        std::abort();
    }
}

inline std::ofstream OpenOutputFileSafety(std::string file_path, std::_Ios_Openmode mode)
{
    std::ofstream file(file_path,mode);
    if(!file.is_open())
    {
        std::cerr << "文件打开失败" << std::endl;
        Log("文件打开失败,文件名：" + file_path);
    }
    return file;
}
template<typename T>
inline T GetParam(T&& val)
{

}

inline std::ifstream OpenInputFileSafety(std::string file_path, std::_Ios_Openmode mode = std::ios::app)
{
    std::ifstream file(file_path,mode);
    if(!file.is_open())
    {
        std::cerr << "文件打开失败" << std::endl;
        Log("文件打开失败,文件名：" + file_path);
    }
    return file;
}

//
// inline void FindMapSafety()