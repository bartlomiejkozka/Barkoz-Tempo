#ifndef DEBUG_H
#define DEBUG_H

#include "DebugCfg.h"
#include "StdCfg.h"
#include "DebugAnsi.h"

#if defined(DEBUG_PRINTF_ENABLE) && (DEBUG_PRINTF_ENABLE == STD_ON)

#define DebugPrintf(...)    printf(__VA_ARGS__)

#define DEBUG_LEVEL_LABEL(_LEVEL_LABEL_)   DebugPrintf("[%s]", _LEVEL_LABEL_)

#if defined(DEBUG_ANSI_COLOR_ENABLE) && (DEBUG_ANSI_COLOR_ENABLE == STD_ON)
#define DEBUG_LEVEL_COLOR(_LEVEL_COLOR_)   DebugPrintf("%s", _LEVEL_COLOR_)
#else
#define DEBUG_LEVEL_COLOR(_LEVEL_COLOR_)
#endif

#if defined(DEBUG_SOURCE_INFO_ENABLE) && (DEBUG_SOURCE_INFO_ENABLE == STD_ON)
#define DEBUG_SOURCE_INFO() DebugPrintf("[%s:%d]", __FILE__, __LINE__)
#else
#define DEBUG_SOURCE_INFO()
#endif

#if defined(DEBUG_FUNCTION_INFO_ENABLE) && (DEBUG_FUNCTION_INFO_ENABLE == STD_ON)
#define DEBUG_FUNCTION_INFO() DebugPrintf("[%s]", __FUNCTION__)
#else
#define DEBUG_FUNCTION_INFO()
#endif

#define DEBUG_ENTRY(_LEVEL_LABEL_, _LEVEL_COLOR_, ...) \
    do {                                               \
        DEBUG_LEVEL_COLOR(_LEVEL_COLOR_);              \
        DEBUG_LEVEL_LABEL(_LEVEL_LABEL_);              \
        DEBUG_SOURCE_INFO();                           \
        DEBUG_FUNCTION_INFO();                         \
        DEBUG_LEVEL_COLOR(ANSI_COLOR_RESET);           \         
        DebugPrintf("  ");                             \
        DebugPrintf(__VA_ARGS__);                      \
        DebugPrintf("\n");                             \
    }while (0)

#if (DEBUG_SEVERITY_LVL >= DEBUG_LVL_ERROR)
#define DEBUG_ERROR(...)    DEBUG_ENTRY("ERROR", ANSI_COLOR_RED, __VA_ARGS__)
#else
#define DEBUG_ERROR(...)
#endif

#if (DEBUG_SEVERITY_LVL >= DEBUG_LVL_WARNING)
#define DEBUG_WARNING(...)  DEBUG_ENTRY("WARNING", ANSI_COLOR_YELLOW, __VA_ARGS__)
#else
#define DEBUG_WARNING(...)
#endif

#if (DEBUG_SEVERITY_LVL >= DEBUG_LVL_INFO)
#define DEBUG_INFO(...)     DEBUG_ENTRY("INFO", ANSI_COLOR_GREEN, __VA_ARGS__)
#else
#define DEBUG_INFO(...)
#endif

#if (DEBUG_SEVERITY_LVL >= DEBUG_LVL_TRACE)
#define DEBUG_TRACE(...)    DEBUG_ENTRY("TRACE", ANSI_COLOR_BLUE, __VA_ARGS__)
#else
#define DEBUG_TRACE(...)
#endif

#else

#define DebugPrintf(...)
#define DEBUG_ERROR(...)
#define DEBUG_WARNING(...)
#define DEBUG_INFO(...)
#define DEBUG_TRACE(...)

#endif

#if defined(DEBUG_BOARD) && (DEBUG_BOARD == STD_ON)
#define DEBUG_BOARD_ERROR(_m, ...)      DEBUG_ERROR("%s", _m, ##__VA_ARGS__)
#define DEBUG_BOARD_WARNING(_m, ...)    DEBUG_WARNING("%s", _m, ##__VA_ARGS__)
#define DEBUG_BOARD_INFO(_m, ...)       DEBUG_INFO("%s", _m, ##__VA_ARGS__)
#define DEBUG_BOARD_TRACE(_m, ...)      DEBUG_TRACE("%s", _m, ##__VA_ARGS__)
#else
#define DEBUG_BOARD_ERROR(_m, ...)
#define DEBUG_BOARD_WARNING(_m, ...)
#define DEBUG_BOARD_INFO(_m, ...)
#define DEBUG_BOARD_TRACE(_m, ...)
#endif

#if defined(DEBUG_BITOPERATION) && (DEBUG_BITOPERATION == STD_ON)
#define DEBUG_BITOPERATION_ERROR(_m, ...)      DEBUG_ERROR("%s", _m, ##__VA_ARGS__)
#define DEBUG_BITOPERATION_WARNING(_m, ...)    DEBUG_WARNING("%s", _m, ##__VA_ARGS__)
#define DEBUG_BITOPERATION_INFO(_m, ...)       DEBUG_INFO("%s", _m, ##__VA_ARGS__)
#define DEBUG_BITOPERATION_TRACE(_m, ...)      DEBUG_TRACE("%s", _m, ##__VA_ARGS__)
#else
#define DEBUG_BITOPERATION_ERROR(_m, ...)
#define DEBUG_BITOPERATION_WARNING(_m, ...)
#define DEBUG_BITOPERATION_INFO(_m, ...)
#define DEBUG_BITOPERATION_TRACE(_m, ...)
#endif

#endif // DEBUG_H
