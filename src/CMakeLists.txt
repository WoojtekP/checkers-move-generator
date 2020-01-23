include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(bitboard bitboard.cpp)
add_library(generator bitboard.cpp moveGenerator.cpp)