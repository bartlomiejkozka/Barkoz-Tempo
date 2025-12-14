pybind11_add_module(PyPerft
    perft_wrapper.cpp
)

target_link_libraries(PyPerft 
    PRIVATE Perft
    PRIVATE Board
    PRIVATE MoveGeneration
    PRIVATE PieceMap
)

add_custom_command(TARGET PyPerft POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:PyPerft>
    ${CMAKE_SOURCE_DIR}/tests/functional_tests/perft/
    COMMENT "Copying PyPerft lib to tests/functional_tests/perft/ directory"
)
