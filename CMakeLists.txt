cmake_minimum_required(VERSION 3.10)

project(Barkoz-Tempo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src/BitOperation)
add_subdirectory(src/Board)
add_subdirectory(src/BoardUtils)
add_subdirectory(src/PieceMap)

add_executable(Barkoz-Tempo src/main.cpp)

target_link_libraries(Barkoz-Tempo
    PRIVATE
        BitOperation
        Board
)