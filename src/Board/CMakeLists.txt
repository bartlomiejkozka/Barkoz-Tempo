add_library(Board src/Board.cpp)

target_include_directories(Board
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries(Board
    PUBLIC 
        BitOperation
        BoardUtils
)