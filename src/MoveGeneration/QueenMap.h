// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
/* Queen moves Map using magics                  */
/*************************************************/

#ifndef QUEEN_MAP
#define QUEEN_MAP

#include <cstdint>
#include <array>

#include "BishopMap.h"
#include "RookMap.h"

class Queen 
{
public:
    //--------------------
    // Initilizers
    //--------------------
    
    Queen() = delete;
    
    //------------------
    // Main API function
    //------------------

    [[nodiscard("PURE FUN")]] static constexpr uint64_t getMoves(const int originSq, const uint64_t bbUs, const uint64_t bbThem)
    {
        return Bishop::getMoves(originSq, bbUs, bbThem) | Rook::getMoves(originSq, bbUs, bbThem);
    }
};

#endif // QUEEN_MAP
