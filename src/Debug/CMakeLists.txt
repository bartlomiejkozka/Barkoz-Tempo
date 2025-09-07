add_library(Debug INTERFACE)

target_include_directories(Debug
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    Debug
    INTERFACE
    Shared
)
