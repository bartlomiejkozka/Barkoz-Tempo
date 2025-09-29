// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
/* Generate code for arrays with magic numbers   */
/*************************************************/

#include "MagicGenerator.hpp"
#include "MagicHolder.hpp"

#include <array>s
#include <utility>

void magicGenerate()
{
    std::array<std::pair<uint64_t, uint8_t>, BIT_BOARD_SQUARES_SIZE> magicsRook;
    std::array<std::pair<uint64_t, uint8_t>, BIT_BOARD_SQUARES_SIZE> magicsBishop;
    
    for (int i = 0; i < BIT_BOARD_SQUARES_SIZE; ++i)
    {
        magicsRook[i] = findMagic(i, count1s(occupanciesMask))
    }

}
