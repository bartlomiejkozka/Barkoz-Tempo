add_library(Perft
    PerftFunctions.cpp
)

target_link_libraries(Perft
    PUBLIC MoveGeneration
    PRIVATE MoveParser
)
