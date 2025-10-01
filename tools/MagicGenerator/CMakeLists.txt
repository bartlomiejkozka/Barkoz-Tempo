cmake_minimum_required(VERSION 4.0)
set(CMAKE_CXX_STANDARD 20)
project(MagicNumberGenerator)

add_library(magicGen)
target_sources(magicGen
    PRIVATE
        MagicGenerator.cpp
    PUBLIC
        MagicGenerator.cpp
        HashMagicFunctions.hpp
        MagicSlider.hpp
)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE magicGen)
