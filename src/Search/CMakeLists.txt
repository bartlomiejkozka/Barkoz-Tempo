add_library(Search
    Search.cpp
)

target_link_libraries(Search
    PUBLIC Evaluation
    PUBLIC MoveGeneration
)
