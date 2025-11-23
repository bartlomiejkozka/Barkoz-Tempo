#ifndef STD_H
#define STD_H

#include "StdCfg.h"

#if USING_GCC
    #include <cstdlib>
    constexpr int ABS (int x) { return std::abs(x); }
#else
    constexpr int ABS(int x) { return x < 0 ? -x : x; }
#endif


#endif
