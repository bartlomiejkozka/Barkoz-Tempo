add_library(Search
    Search.cpp
)

target_link_libraries(Search
    PRIVATE Evaluation
    PUBLIC MoveGeneration
)
