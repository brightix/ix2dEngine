cmake_minimum_required(VERSION 4.0)
project(ix2dNew)

set(CMAKE_CXX_STANDARD 23)

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
target_include_directories(ix2d PRIVATE
        "${CMAKE_SOURCE_DIR}/Source/SDL_Lib/SDL3-3.2.20/include"
        "${CMAKE_SOURCE_DIR}/Source/SDL_Lib/SDL3_ttf-3.1.0/include"
        "${CMAKE_SOURCE_DIR}/Source/SDL_Lib/SDL3_image/include"
        "${CMAKE_SOURCE_DIR}/Source/Engine"
        "${CMAKE_SOURCE_DIR}/Content"
)

find_library(SDL3_LIB SDL3 PATHS "${CMAKE_SOURCE_DIR}/Source/SDL_Lib/SDL3-3.2.20/lib")
find_library(SDL3TTF_LIB SDL3_ttf PATHS "${CMAKE_SOURCE_DIR}/Source/SDL_Lib/SDL3_ttf-3.1.0//lib")
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

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
#set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")