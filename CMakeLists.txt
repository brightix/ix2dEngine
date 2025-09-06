cmake_minimum_required(VERSION 4.0)
project(ix2dNew)

set(CMAKE_CXX_STANDARD 23)

# 工程源码
file(GLOB ENGINE_SOURCES
        ${CMAKE_SOURCE_DIR}/Source/Engine/Private/*.cpp
        ${CMAKE_SOURCE_DIR}/Source/Engine/Private/Component/*.cpp
        ${CMAKE_SOURCE_DIR}/Source/Content/Public/Component/*.cpp
        ${CMAKE_SOURCE_DIR}/Source/Content/Private/Component/*.cpp
)

# 这里的 main.cpp 相当于 ${file}
add_executable(MyProject
        ${ENGINE_SOURCES}
        ${CMAKE_SOURCE_DIR}/main.cpp
)

# 头文件搜索路径
target_include_directories(MyProject PRIVATE
        "D:/visual studio code/SDL3-3.2.20/x86_64-w64-mingw32/include"
        "D:/visual studio code/SDL3_ttf-3.1.0/x86_64-w64-mingw32/include"
        "${CMAKE_SOURCE_DIR}/Source/Engine"
#        "${CMAKE_SOURCE_DIR}/Source/Engine/Types"
#        "${CMAKE_SOURCE_DIR}/Source/Engine/Classes"
#        "${CMAKE_SOURCE_DIR}/Source/Engine/Utilities"
#        "${CMAKE_SOURCE_DIR}/Source/Engine/Enum"
#        "${CMAKE_SOURCE_DIR}/Source/Engine/Structure"
#        "${CMAKE_SOURCE_DIR}/Source/Engine/Window"
        "${CMAKE_SOURCE_DIR}/Source/Content"
)

# 链接库目录
link_directories(
        "D:/visual studio code/SDL3-3.2.20/x86_64-w64-mingw32/lib"
        "D:/visual studio code/SDL3_ttf-3.1.0/x86_64-w64-mingw32/lib"
)

find_library(SDL3_LIB SDL3 PATHS "D:/visual studio code/SDL3-3.2.20/x86_64-w64-mingw32/lib")
find_library(SDL3TTF_LIB SDL3_ttf PATHS "D:/visual studio code/SDL3_ttf-3.1.0/x86_64-w64-mingw32/lib")

set_target_properties(MyProject PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# 链接库
target_link_libraries(MyProject PRIVATE
        ${SDL3_LIB}
        ${SDL3TTF_LIB}
        winmm
)

add_custom_command(TARGET MyProject POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "D:/visual studio code/SDL3-3.2.20/x86_64-w64-mingw32/bin/SDL3.dll"
        $<TARGET_FILE_DIR:MyProject>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "D:/visual studio code/SDL3_ttf-3.1.0/x86_64-w64-mingw32/bin/SDL3_ttf.dll"
        $<TARGET_FILE_DIR:MyProject>
)