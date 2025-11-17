add_library(Debug INTERFACE)

# here we add tha path to dir although we have here 3 include files, but only one will be used in external includes "debug.h"
target_include_directories(Debug
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Debug
    INTERFACE Shared
)
