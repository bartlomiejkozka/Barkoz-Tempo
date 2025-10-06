add_library(PieceMap INTERFACE)

target_include_directories(PieceMap
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(PieceMap
    INTERFACE Board
)
