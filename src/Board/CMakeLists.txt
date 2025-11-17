add_library(Board 
    Board.cpp
)

# see CMakeLists file comment in BitOperation module
target_include_directories(Board
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Board
    PUBLIC BitOperation
    PUBLIC MoveGeneration
    PRIVATE PieceMap
)
