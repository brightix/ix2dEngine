cmake_minimum_required(VERSION 3.30.5)
project(ix2dNew)

set(CMAKE_CXX_STANDARD 23)

# 工程源码
file(GLOB_RECURSE ENGINE_SOURCES
        ${CMAKE_SOURCE_DIR}/Source/Engine/Private/*.cpp
        ${CMAKE_SOURCE_DIR}/Source/Content/Private/*.cpp
#        ${CMAKE_SOURCE_DIR}/Source/Content/Public/ActorComponent/*.cpp
)
add_executable(ix2d
        ${ENGINE_SOURCES}
        ${CMAKE_SOURCE_DIR}/main.cpp
)

# 头文件搜索路径
target_include_directories(ix2d PRIVATE
        "C:/toolLibs/SDL3-3.2.22/x86_64-w64-mingw32/include"
        "C:/toolLibs/SDL3_ttf-3.2.2/x86_64-w64-mingw32/include"
        "${CMAKE_SOURCE_DIR}/Source/Engine"
        "${CMAKE_SOURCE_DIR}/Content/Public"
)

# 链接库目录
link_directories(
        "C:/toolLibs/SDL3-3.2.22/x86_64-w64-mingw32/lib"
        "C:/toolLibs/SDL3_ttf-3.2.2/x86_64-w64-mingw32/lib"
)

find_library(SDL3_LIB SDL3 PATHS "C:/toolLibs/SDL3-3.2.22/x86_64-w64-mingw32/lib")
find_library(SDL3TTF_LIB SDL3_ttf PATHS "C:/toolLibs/SDL3_ttf-3.2.2/x86_64-w64-mingw32/lib")

set_target_properties(ix2d PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# 链接库
target_link_libraries(ix2d PRIVATE
        ${SDL3_LIB}
        ${SDL3TTF_LIB}
        winmm
)

add_custom_command(TARGET ix2d POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/toolLibs/SDL3-3.2.22/x86_64-w64-mingw32/bin/SDL3.dll"
        $<TARGET_FILE_DIR:ix2d>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/toolLibs/SDL3_ttf-3.2.2/x86_64-w64-mingw32/bin/SDL3_ttf.dll"
        $<TARGET_FILE_DIR:ix2d>
)


#        "${CMAKE_SOURCE_DIR}/Source/Engine/Types"
#        "${CMAKE_SOURCE_DIR}/Source/Engine/Classes"
#        "${CMAKE_SOURCE_DIR}/Source/Engine/Utilities"
#        "${CMAKE_SOURCE_DIR}/Source/Engine/Enum"
#        "${CMAKE_SOURCE_DIR}/Source/Engine/Structure"
#        "${CMAKE_SOURCE_DIR}/Source/Engine/Window"