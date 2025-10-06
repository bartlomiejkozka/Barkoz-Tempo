add_library(MoveGenerator INTERFACE)

target_link_libraries(MoveGenerator
    INTERFACE Board
    INTERFACE Debug
    INTERFACE BitOperation
)
