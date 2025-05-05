#ifndef DEBUG_ANSI_H
#define DEBUG_ANSI_H

#include "DebugCfg.h"
#include "StdCfg.h"

#if defined(DEBUG_ANSI_COLOR_ENABLE) && (DEBUG_ANSI_COLOR_ENABLE == STD_ON)

#define ANSI_COLOR_START   "\033["
#define ANSI_COLOR_RESET   "\033[0m"
#define ANSI_COLOR(color)  ANSI_COLOR_START color "m"

#define ANSI_COLOR_BLACK   "30"
#define ANSI_COLOR_RED     "31"
#define ANSI_COLOR_GREEN   "32"
#define ANSI_COLOR_YELLOW  "33"
#define ANSI_COLOR_BLUE    "34"

#else

#define ANSI_COLOR_START
#define ANSI_COLOR_RESET
#define ANSI_COLOR(color)

#define ANSI_COLOR_BLACK
#define ANSI_COLOR_RED
#define ANSI_COLOR_GREEN
#define ANSI_COLOR_YELLOW
#define ANSI_COLOR_BLUE

#endif

#endif // DEBUG_ANSI_H