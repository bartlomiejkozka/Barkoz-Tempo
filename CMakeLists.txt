cmake_minimum_required(VERSION 3.10)

project(Barkoz-Tempo)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Barkoz-Tempo src/main.cpp)

# set_target_properties(Barkoz-Tempo PROPERTIES
#   CMAKE_CXX_STANDARD 20
#   CMAKE_CXX_STANDARD_REQUIRED ON  # compiler must use C++ specified version
#   CMAKE_CXX_EXTENSIONS OFF        # compiler can use only ISO C++ version specifier (GNU extensions not used)
# )

set(MODULE_DIRS
  src/BitOperation
  src/Board
  src/BoardUtils
  src/PieceMap
  src/StateInfo
  src/Shared
  src/Debug
  src/MoveGenerator
)

foreach(d IN LISTS MODULE_DIRS)
  add_subdirectory(${d})
endforeach()

target_link_libraries(Barkoz-Tempo
    PRIVATE
        BitOperation
        Board
        StateInfo
        PieceMap
        MoveGenerator
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