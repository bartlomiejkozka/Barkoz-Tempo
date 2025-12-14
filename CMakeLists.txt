cmake_minimum_required(VERSION 3.10)

project(Barkoz-Tempo)

option(BUILD_PYTHON_BINDINGS "Build Python bindings for testing" OFF)

if(BUILD_PYTHON_BINDINGS)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    message(STATUS "Building in test mode (Python bindings enabled, PIC ON)")
else()
    message(STATUS "Building in engine mode (Max performance, PIC OFF)")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_definitions(_GLIBCXX_DEBUG=0)

add_executable(Barkoz-Tempo src/main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_definitions(USING_GCC=1)
else()
    add_compile_definitions(USING_GCC=0)
endif()


set(MODULE_DIRS
	src/BitOperation
	src/Board
	src/BoardUtils
	src/Debug
	src/MoveGeneration
	src/PieceMap
	src/Shared
	src/StateInfo
	src/MoveGeneration/MoveParser
	src/MoveGeneration/Perft
	src/Engine
	src/Uci
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

foreach(d IN LISTS MODULE_DIRS)
	add_subdirectory(${d})
endforeach()

target_link_libraries(Barkoz-Tempo
    PRIVATE
        BitOperation
        Board
        MoveGeneration
		PieceMap
		MoveParser
		Engine
		Uci
)


# -------------------------------------------
# TESTS
# -------------------------------------------

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
	MoveGeneration
)

include(GoogleTest)
gtest_discover_tests(moveUtility_test)

# functional_tests - pytests - perft

if(BUILD_PYTHON_BINDINGS)
	add_subdirectory(extern/pybind11)
	add_subdirectory(tests/functional_tests/perft/cpp_bindings)
endif()
