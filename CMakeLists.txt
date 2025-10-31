cmake_minimum_required(VERSION 4.0)
project(ix2dNew)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 定义一个可控开关变量，用于区分 Debug / Release
option(DebugMode "Enable debug build" OFF)
if (MSVC)
    if(DebugMode)
        message(STATUS "Compiler: MSVC detected")
        add_compile_options("/utf-8" "/Zi" "/EHsc")
        add_link_options("/DEBUG")
    else()
        add_compile_options(
                /utf-8          # 源码用 UTF-8 编码
                /O2             # 最大优化（速度）
                /GL             # 全程序优化 (Whole Program Optimization)
                /fp:fast        # 快速浮点计算（非精确）
                /DNDEBUG        # 禁用断言
                /Gy             # 函数级链接（配合 LTCG）
                /EHsc           # 标准异常处理
                /arch:AVX2      # 使用 AVX2 指令集（你的 CPU 支持的话）
                /Oi             # 内联 intrinsic 函数
        )
        # 链接优化
        add_link_options(
                /LTCG           # 链接时代码生成 (Link Time Code Generation)
                /OPT:REF        # 删除未使用函数/数据
                /OPT:ICF        # 合并相同函数
                /INCREMENTAL:NO # 禁用增量链接，获得完整优化
        )
    endif ()
else()
    if(DebugMode)
        message(STATUS "Compiler: GCC/Clang detected")
        add_compile_options(-fsanitize=address -g -O1 -Wall -Wextra)
        add_link_options(-fsanitize=address)
    else()
        add_compile_options(
                -O3             # 最大优化级别
                -march=native   # 针对本机 CPU 指令集优化
                -funroll-loops  # 展开循环
                -fomit-frame-pointer
                -fstrict-aliasing
                -ffast-math      # 快速浮点计算
                -DNDEBUG
        )
        add_link_options(
                -flto           # Link Time Optimization
                -s              # 移除符号表，减小体积
        )
    endif ()
endif()
# 工程源码
file(GLOB_RECURSE ENGINE_SOURCES
        ${CMAKE_SOURCE_DIR}/Source/Engine/Private/*.cpp
        ${CMAKE_SOURCE_DIR}/Source/Content/Private/*.cpp
        ${CMAKE_SOURCE_DIR}/Content/Private/*.cpp
)

add_definitions(-D_CRTDBG_MAP_ALLOC)
add_executable(ix2d
        ${ENGINE_SOURCES}
        ${CMAKE_SOURCE_DIR}/main.cpp
)
set_target_properties(ix2d PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)
# 头文件搜索路径

target_compile_options(ix2d PRIVATE
        $<$<CONFIG:Debug>:/MDd>
        $<$<CONFIG:Release>:/MD>
)
target_include_directories(ix2d PRIVATE
        "${CMAKE_SOURCE_DIR}/Source/SDL_Lib/SDL3-3.2.20/include"
        "${CMAKE_SOURCE_DIR}/Source/SDL_Lib/SDL3_ttf-3.1.0/include"
        "${CMAKE_SOURCE_DIR}/Source/SDL_Lib/SDL3_image/include"
        "${CMAKE_SOURCE_DIR}/Source/Engine"
        "${CMAKE_SOURCE_DIR}/Content"
)

find_library(SDL3_LIB SDL3 PATHS "${CMAKE_SOURCE_DIR}/Source/SDL_Lib/SDL3-3.2.20/lib")
find_library(SDL3TTF_LIB SDL3_ttf PATHS "${CMAKE_SOURCE_DIR}/Source/SDL_Lib/SDL3_ttf-3.1.0/lib")
#find_library(SDL3IMAGE_LIB SDL3_image PATHS "E:/ix/ix2dEngine-main/ix2dEngine/Source/SDL_Lib/SDL3_image/lib")
#
find_library(SDL3IMAGE_LIB SDL3_image PATHS "${CMAKE_SOURCE_DIR}/Source/SDL_Lib/SDL3_image/lib")


# 链接库
target_link_libraries(ix2d PRIVATE
        ${SDL3IMAGE_LIB}
        ${SDL3_LIB}
        ${SDL3TTF_LIB}
        winmm
)
