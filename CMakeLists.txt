cmake_minimum_required(VERSION 3.10)

project(Barkoz-Tempo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src/BitOperation)
add_subdirectory(src/Board)
add_subdirectory(src/BoardUtils)
add_subdirectory(src/PieceMap)
add_subdirectory(src/StateInfo)
add_subdirectory(src/Shared)
add_subdirectory(src/Debug)
add_subdirectory(src/MoveGenerator)

add_executable(Barkoz-Tempo src/main.cpp)

target_link_libraries(Barkoz-Tempo
    PRIVATE
        BitOperation
        Board
        StateInfo
        PieceMap
)

# tests

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  moveUtility_test
  tests/unit_tests/moveUtility_test.cc
)
target_link_libraries(
  moveUtility_test
  GTest::gtest_main
  MoveGenerator
)

include(GoogleTest)
gtest_discover_tests(moveUtility_test)