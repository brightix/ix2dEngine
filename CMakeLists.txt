cmake_minimum_required(VERSION 4.0)
project(ix2dNew)

set(CMAKE_CXX_STANDARD 23)
# 定义一个可控开关变量，用于区分 Debug / Release
option(DebugMode "Enable debug build" ON)

# 工程源码
file(GLOB_RECURSE ENGINE_SOURCES
        ${CMAKE_SOURCE_DIR}/Source/Engine/Private/*.cpp
        ${CMAKE_SOURCE_DIR}/Source/Content/Private/*.cpp
        ${CMAKE_SOURCE_DIR}/Content/Private/*.cpp
)

add_executable(ix2d
        ${ENGINE_SOURCES}
        ${CMAKE_SOURCE_DIR}/main.cpp
)
set_target_properties(ix2d PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}"
)





if(MSVC)
    # 通用 MSVC 编译选项
    target_compile_options(ix2d PRIVATE
            /utf-8
            /Od
            /Zi
            /RTC1
            /GS-
            /Oy-
    )
#    target_compile_options(ix2d PRIVATE
#            /utf-8
#            /permissive-
#            $<$<CONFIG:Debug>:/Zi /Od /Zo /Zf /Oy- /fp:fast /GS- /MDd>
#            #$<$<CONFIG:Release>:/Oi /Ob2 /arch:AVX2 /Gy /DNDEBUG /Gw /MD>
#    )
    # Debug 模式下启用内存泄漏检测
    if(DebugMode)
        #add_definitions(-D_CRTDBG_MAP_ALLOC)
    endif()
else()
    # MinGW / GCC / Clang
    target_compile_options(ix2d PRIVATE
            $<$<CONFIG:Debug>:-O0 -g -fexceptions>
            $<$<CONFIG:Release>:-O3 -DNDEBUG -march=native -flto -fomit-frame-pointer>
    )
endif()


# 头文件搜索路径


target_include_directories(ix2d PRIVATE
        "${CMAKE_SOURCE_DIR}/Source/SDL_Lib/SDL3-3.2.20/include"
        "${CMAKE_SOURCE_DIR}/Source/SDL_Lib/SDL3_ttf-3.1.0/include"
        "${CMAKE_SOURCE_DIR}/Source/SDL_Lib/SDL3_image/include"
        "${CMAKE_SOURCE_DIR}/Source/Engine"
        "${CMAKE_SOURCE_DIR}/Content"
)

find_library(SDL3_LIB SDL3 PATHS "${CMAKE_SOURCE_DIR}/Source/SDL_Lib/SDL3-3.2.20/lib")
find_library(SDL3TTF_LIB SDL3_ttf PATHS "${CMAKE_SOURCE_DIR}/Source/SDL_Lib/SDL3_ttf-3.1.0/lib")
find_library(SDL3IMAGE_LIB SDL3_image PATHS "${CMAKE_SOURCE_DIR}/Source/SDL_Lib/SDL3_image/lib")


# 链接库
target_link_libraries(ix2d PRIVATE
        ${SDL3IMAGE_LIB}
        ${SDL3_LIB}
        ${SDL3TTF_LIB}
        winmm
)

#message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
#message(STATUS "DebugMode: ${DebugMode}")
#message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")