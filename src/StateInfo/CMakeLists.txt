add_library(StateInfo INTERFACE)

target_include_directories(StateInfo
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(StateInfo
    INTERFACE Board
)
