#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include "Utilities/magic_enum.hpp"
#include "Enum/LogLevel.h"

inline void LogToFile(const std::string str, LogLevel level = LogLevel::Tip)
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
    std::string logText = timeStream.str();

    logText.append(" [" + std::string(magic_enum::enum_name(level)) + "]: " + str);
    file << logText << std::endl;
    file.close();
    if (level == FatalError)
    {
        std::cerr << "致命错误" << std::endl;
        std::abort();
    }
}

inline void PrintString(std::string str)
{


}