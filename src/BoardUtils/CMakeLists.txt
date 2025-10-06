add_library(BoardUtils INTERFACE)

target_include_directories(BoardUtils
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(BoardUtils
    INTERFACE Board   # included by header
)
