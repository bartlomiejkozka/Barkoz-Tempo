#ifndef STD_H
#define STD_H

#include "StdCfg.h"

#if USING_GCC
    #include <cstdlib>
    INLINE int ABS (int x) { return std::abs(x); }
#else
    INLINE constexpr int ABS(int x) { return x < 0 ? -x : x; }
#endif


#endif
