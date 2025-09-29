// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
/* Generate code for arrays with magic numbers   */
/*************************************************/

#ifndef MAGIC_GENERATOR
#define MAGIC_GENERATOR

#include <cstdint>

#define BIT_BOARD_SQUARES_SIZE  (64)

int count1s(uint64_t num)
{
    int r;
    for (r = 0; num != 0; ++r, num &= num - 1);
    return r;
}

void magicGenerate();

#endif /* MAGIC_GENERATOR */
