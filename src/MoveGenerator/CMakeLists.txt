add_library(MoveGenerator INTERFACE)

target_include_directories(
    MoveGenerator
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    MoveGenerator
    INTERFACE
    Board
    Debug
)
