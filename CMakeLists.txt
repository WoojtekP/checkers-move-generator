cmake_minimum_required(VERSION 2.8)
project(checkers_move_generator)

set(CMAKE_CXX_FLAGS "-O3 -Wall")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/)

add_subdirectory(src)
add_subdirectory(test)

add_executable(benchmark src/benchmark.cpp)
target_link_libraries(benchmark generator)