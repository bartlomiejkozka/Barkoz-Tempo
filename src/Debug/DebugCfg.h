#ifndef DEBUG_CFG_H
#define DEBUG_CFG_H

#include "StdCfg.h"

//--------DEBUG SEVERITY LEVELS--------
#define DEBUG_LVL_DISABLED  (0U)
#define DEBUG_LVL_ERROR     (1U)
#define DEBUG_LVL_WARNING   (2U)
#define DEBUG_LVL_INFO      (3U)
#define DEBUG_LVL_TRACE     (4U)

// ==========================================================
// =============== DEBUG CONFIGURATION ======================
// ==========================================================

#define DEBUG_ENABLE (STD_ON)

#if defined(DEBUG_ENABLE) && DEBUG_ENABLE == 1

#define DEBUG_PRINTF_ENABLE          (STD_ON)
#define DEBUG_ANSI_COLOR_ENABLE      (STD_ON)
#define DEBUG_SOURCE_INFO_ENABLE     (STD_ON)
#define DEBUG_FUNCTION_INFO_ENABLE   (STD_ON)
#define DEBUG_SEVERITY_LVL           (DEBUG_LVL_INFO)

#define DEBUG_BOARD         (STD_ON)
#define DEBUG_BITOPERATION  (STD_ON)

#else

#define DEBUG_PRINTF_ENABLE          (STD_OFF)
#define DEBUG_ANSI_COLOR_ENABLE      (STD_OFF)
#define DEBUG_SOURCE_INFO_ENABLE     (STD_OFF)
#define DEBUG_FUNCTION_INFO_ENABLE   (STD_OFF)
#define DEBUG_LVL                    (DEBUG_LVL_DISABLED)

#define DEBUG_BOARD         (STD_OFF)
#define DEBUG_BITOPERATION  (STD_OFF)
#define BOARD_UTILS         (STD_OFF)

#endif

#endif // DEBUG_CFG_H
