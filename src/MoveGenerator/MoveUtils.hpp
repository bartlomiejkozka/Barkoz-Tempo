#ifndef MOVE_UTILS_HPP
#define MOVE_UTILS_HPP

#include <array>
#include <cstdint>

#include "Board.hpp"


/*
2D Array containing hard-coded bitbards with bits set that are between (vertical, horizoontal, diagonal) from-to indicies

* For the not hirozontal, vertical or diagonal squares array value is set to zeros bit board
*/

static const std::array<std::array<uint64_t, Board::boardSize>, Board::boardSize> inBetween = [] () {
    std::array<std::array<uint64_t, Board::boardSize>, Board::boardSize> tab = {};

    for(int i = 0; i < Board::boardSize; ++i)
    {
        for(int j = 0; j < Board::boardSize; ++j)
        {
            if (i / 8 == j / 8 || i % 8 == j % 8)
            {
                tab[i][j] = static_cast<uint64_t>(2 ^ (j - i) - 1) << i;
            }
            else if (abs(i/8 - j/8) == abs(i%8 - j%8))
            {
                for (int d = 0; d < abs(i/8 - j/8); ++d)
                {
                    tab[i][j] |= static_cast<uint64_t>(1) << 9*(d+1) + i;
                }
            }
            else
            {
                // alredy assign to 0
            }
        }
    }

    return tab;
}();



#endif
