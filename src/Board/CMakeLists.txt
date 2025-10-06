add_library(Board INTERFACE)

# see CMakeLists file comment in BitOperation module
target_include_directories(Board
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Board
    INTERFACE BitOperation    # have to be public -> h file use this
)
