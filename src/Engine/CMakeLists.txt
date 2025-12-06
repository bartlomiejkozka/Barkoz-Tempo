add_library(Engine 
    Evaluation.cpp
    Search.cpp
)

target_link_libraries(Engine
    PRIVATE BitOperation
    PUBLIC MoveGeneration
    PRIVATE MoveParser
    PUBLIC Board
)
