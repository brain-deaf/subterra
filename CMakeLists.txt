cmake_minimum_required(VERSION 3.23)
project(psx_hello_world LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Pull in vcpkg‐installed SDL3 and bgfx
find_package(SDL3 CONFIG REQUIRED)
find_package(bgfx CONFIG REQUIRED)

# Our executable
add_executable(psx_hello_world
  Subterra.cpp
)

# Link exactly the imported targets
target_link_libraries(psx_hello_world
  PRIVATE
    SDL3::SDL3       # from SDL3Config.cmake
    bgfx::bx         # utility lib
    bgfx::bimg       # image helpers
    bgfx::bgfx       # the renderer
)
