cmake_minimum_required(VERSION 4.0)
project(ix2dNew)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    message(STATUS "Compiler: MSVC detected")
    add_compile_options("/utf-8" "/Zi" "/EHsc" "/MDd")
    add_link_options("/DEBUG")
else()
    message(STATUS "Compiler: GCC/Clang detected")
    add_compile_options(-fsanitize=address -g -O1 -Wall -Wextra)
    add_link_options(-fsanitize=address)
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

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
#set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")