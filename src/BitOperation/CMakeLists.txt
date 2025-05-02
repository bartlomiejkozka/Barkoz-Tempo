add_library(BitOperation
    src/BitOperation.cpp
)

target_include_directories(BitOperation
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)