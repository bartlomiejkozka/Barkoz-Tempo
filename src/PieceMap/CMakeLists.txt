add_library(PieceMap
    PieceMap.cpp
)

target_include_directories(PieceMap
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(PieceMap
    PUBLIC Board
)
