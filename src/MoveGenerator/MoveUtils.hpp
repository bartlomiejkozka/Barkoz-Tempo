#ifndef MOVE_UTILS_HPP
#define MOVE_UTILS_HPP

#include <array>
#include <cstdint>
#include <cmath>

#include "Board.hpp"

/* 
* Usage of inline static varaible is neccessary to avoid the linkage error.
* Up to c++17 is possiblity to attach the same static var to different TU across program.
* Before it was not possible due to linker err.
*/

struct MoveUtils {

    //------------------
    // Initializators
    //------------------

    MoveUtils() = delete;
        
    //------------------
    // Attrib utils
    //------------------

    [[nodiscard("PURE FUN")]] static constexpr uint64_t empty(const uint64_t board) { return ~board; }

    /*
    2D Array containing hard-coded bitbards with bits set that are between (vertical, horizoontal, diagonal) from-to indicies

    * For the not hirozontal, vertical or diagonal squares array value is set to zeros bit board
    */
    inline static const std::array<std::array<uint64_t, Board::boardSize>, Board::boardSize> inBetween = [] () {
        std::array<std::array<uint64_t, Board::boardSize>, Board::boardSize> tab = {};

        for(int i = 0; i < Board::boardSize; ++i)
        {
            for(int j = 0; j < Board::boardSize; ++j)
            {
                if (i == j)
                {
                    continue;
                }
                int min = (i < j ? i : j);
                int max = (i > j ? i : j);
                if (i / 8 == j / 8)
                {
                    tab[i][j] = static_cast<uint64_t>((1ULL << (abs(i - j) - 1)) - 1) << (min + 1);
                }
                else if (i % 8 == j % 8)
                {
                    for (int it = min+8; it < max; it+=8) 
                    {
                        tab[i][j] |= static_cast<uint64_t>(1) << it;
                    }
                }
                else if (abs(i/8 - j/8) == abs(i%8 - j%8))
                {
                    int diag = 1;
                    if (min % 8 > max % 8)
                    {
                        diag = -1;
                    }
                    for (int it = min+8+diag; it < max; it+=8+diag) 
                    {
                        tab[i][j] |= static_cast<uint64_t>(1) << it;
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

};



#endif
