add_library(MoveGeneration
    ChessRules.cpp
    MoveGenerator.cpp
)

target_link_libraries(MoveGeneration
    PUBLIC Board
    PRIVATE Debug
    PUBLIC BitOperation
)
