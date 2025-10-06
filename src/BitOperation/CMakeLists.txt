add_library(BitOperation INTERFACE)

# there is no need to type including "BitOperation/BitOperation" when is only 1 file module
# so could include this directory
target_include_directories(BitOperation
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)
