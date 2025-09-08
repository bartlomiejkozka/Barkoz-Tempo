add_library(HashMagicFunction INTERFACE)

target_include_directories(
    HashMagicFunction
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    HashMagicFunction
    INTERFACE
    BitOperation
    Debug
    MoveGenerator
)
