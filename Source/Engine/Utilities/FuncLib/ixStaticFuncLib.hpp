#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include "Utilities/ThirdParty/magic_enum.hpp"
#include "Enum/LogLevel.h"
#include "../ThirdParty/json.hpp"
#include "Utilities/FuncLib/GlobalMacros.hpp"
#define Log_f(msg,...) LogToFile(__FILE__, __LINE__, __func__,LogLevel::Tip, std::format(msg,##__VA_ARGS__))
#define Log(msg) LogToFile(__FILE__, __LINE__, __func__,LogLevel::Tip, msg)
#define LogWithLevel(level,msg,...) LogToFile(__FILE__, __LINE__, __func__, level,msg)
#define LogWithLevel_f(level,msg,...) LogToFile(__FILE__, __LINE__, __func__, level,std::format(msg,##__VA_ARGS__))
#define CAST(x) Cast(x,__func__)
inline void LogToFile(const char* file_name = "", const int line = 0, const char* func_name = "", const LogLevel level = Tip,const std::string& msg = "")
{
	static std::ofstream file("Log.log", std::ios::app);
	if(!file.is_open()) std::cerr << "文件打开失败" << std::endl;

	auto now = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);
	std::tm tm = *std::localtime(&now_c);

	std::ostringstream oss;
	oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S")
		<< " " << file_name << " at " << line << " line " << func_name
		<< " [" << magic_enum::enum_name(level) << "]: " << msg;
	std::string logText = oss.str();

#if DEBUG == 1
	std::cout << logText << std::endl;
#endif

	file << logText << std::endl;

	if (level == LogLevel::FatalError)
	{
		std::cerr << "致命错误" << std::endl;
		std::abort();
	}
}

inline std::ofstream OpenOutputFileSafety(std::string file_path, std::ios::openmode mode)
{
    std::ofstream file(file_path,mode);
    if(!file.is_open())
    {
        std::cerr << "文件打开失败" << std::endl;
        Log("文件打开失败,文件名：" + file_path);
    }
    return file;
}

inline std::ifstream OpenInputFileSafety(std::string file_path, std::ios::openmode mode = std::ios::app)
{
    std::ifstream file(file_path,mode);
    if(!file.is_open())
    {
        std::cerr << "文件打开失败" << std::endl;
        Log("文件打开失败,文件名：" + file_path);
    }
    return file;
}

template<typename T, typename U>
T* Cast(U* before, const char* func_name = "")
{
	// 静态类型检查
	static_assert(
		std::is_base_of_v<T, U> ||
		std::is_base_of_v<U, T> ||
		std::is_same_v<T, U>,
				  "Cast 检测出类型不兼容：目标类型不是源类型的基类");

	if (!before)
	{
		std::cerr << func_name << " >> 指针为空，无法进行转换\n";
		return nullptr;
	}

	// 如果类型相同，直接返回
	if constexpr (std::is_same_v<T, U>)
		return before;

	// 尝试 dynamic_cast
	if (auto t = dynamic_cast<T*>(before))
		return t;

	std::cerr << func_name << " >> dynamic_cast 失败："
			  << typeid(U).name() << " → " << typeid(T).name() << "\n";
	return nullptr;
}

inline void STOP()
{
	std::cout << "stop" << std::endl;
}


//
// inlin

// inline std::ofstream OpenOutputFileSafety(std::string file_path, std::_Ios_Openmode mode)
// {
//     std::ofstream file(file_path,mode);
//     if(!file.is_open())
//     {
//         std::cerr << "文件打开失败" << std::endl;
//         Log("文件打开失败,文件名：" + file_path);
//     }
//     return file;
// }
// template<typename T>
// inline T GetParam(T&& val)
// {
//
// }
//
// inline std::ifstream OpenInputFileSafety(std::string file_path, std::_Ios_Openmode mode = std::ios::app)
// {
//     std::ifstream file(file_path,mode);
//     if(!file.is_open())
//     {
//         std::cerr << "文件打开失败" << std::endl;
//         Log("文件打开失败,文件名：" + file_path);
//     }
//     return file;
// }
//
// //
// // inline void FindMapSafety()