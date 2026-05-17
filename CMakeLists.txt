cmake_minimum_required(VERSION 3.10)
project(Rasterizer)
add_executable(Rasterizer src/main.cpp src/Vec2.cpp src/Renderer.cpp)
include(FetchContent)
FetchContent_Declare(
    minifb
    GIT_REPOSITORY https://github.com/emoon/minifb.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(minifb)
target_link_libraries(Rasterizer minifb)