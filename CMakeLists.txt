cmake_minimum_required(VERSION 3.14)
project(snek VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_subdirectory(src)