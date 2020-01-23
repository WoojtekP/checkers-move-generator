add_executable(bitboardTests bitboardTests.cpp)
target_link_libraries(bitboardTests bitboard)

add_executable(perft perft.cpp)
target_link_libraries(perft generator)