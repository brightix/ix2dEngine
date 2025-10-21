cmake_minimum_required(VERSION 4.0)
project(ix2dNew)

set(CMAKE_CXX_STANDARD 23)

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
)
# 头文件搜索路径
target_include_directories(ix2d PRIVATE
        "E:/ix/SDL3_ttf-3.2.2/x86_64-w64-mingw32/include"
        "E:/ix/SDL3-3.2.24/x86_64-w64-mingw32/include"
        "${CMAKE_SOURCE_DIR}/Source/SDL_Lib/SDL3_image/include"
        "${CMAKE_SOURCE_DIR}/Source/Engine"
        "${CMAKE_SOURCE_DIR}/Content"
)

find_library(SDL3_LIB SDL3 PATHS "E:/ix/SDL3-3.2.24/x86_64-w64-mingw32/lib")
find_library(SDL3TTF_LIB SDL3_ttf PATHS "E:/ix/SDL3_ttf-3.2.2/x86_64-w64-mingw32/lib")
#find_library(SDL3IMAGE_LIB SDL3_image PATHS "E:/ix/ix2dEngine-main/ix2dEngine/Source/SDL_Lib/SDL3_image/lib")
#
find_library(SDL3IMAGE_LIB SDL3_image PATHS "E:/ix/SDL3_image-3.2.4/x86_64-w64-mingw32/lib")


# 链接库
target_link_libraries(ix2d PRIVATE
        ${SDL3IMAGE_LIB}
        ${SDL3_LIB}
        ${SDL3TTF_LIB}
        winmm
)