add_library(Uci
    Uci.cpp
)

target_link_libraries(Uci
    PUBLIC Board
    PUBLIC Engine
    PRIVATE MoveGeneration
    PRIVATE MoveParser
)
