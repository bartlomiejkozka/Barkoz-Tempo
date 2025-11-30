add_library(Evaluation 
    Evaluation.cpp
)

target_include_directories(Evaluation
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Evaluation
    PUBLIC BitOperation
    PUBLIC MoveGeneration
    PUBLIC Board
)
