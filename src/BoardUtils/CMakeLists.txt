add_library(BoardUtils BoardUtils.cpp)

target_include_directories(BoardUtils
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(BoardUtils
    PUBLIC Board
)