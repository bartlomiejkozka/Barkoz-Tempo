#ifndef MOVE_UTILS_HPP
#define MOVE_UTILS_HPP

#include <array>
#include <cstdint>
#include <cmath>

#include "Board.hpp"
#include "BitOperation.hpp"

/* 
* Usage of inline static varaible is neccessary to avoid the linkage error.
* Up to c++17 is possiblity to attach the same static var to different TU across program.
* Before, it was not possible due to linker err.
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

    // recomended shift funciton when dealing with negative shift values (i.e. in relative moves)
    // left shift  : positives
    // right shift : negatives
    [[nodiscard("PURE FUN")]] static constexpr uint64_t shift(uint64_t x, int shiftBy) { return shiftBy < 0 ? (x >> shiftBy) : (x << shiftBy); } 

    // -------------------
    // All Use
    // -------------------

    [[nodiscard]] static constexpr uint64_t bitBoardSet(int square) { return minBitSet << square; }

    // -------------------
    // King/Knight Use
    // -------------------

    /*
    * Generation of static moves table
    * This function can also be used to gen attackstTo table !
    */
    template<size_t N>
    static consteval std::array<uint64_t, Board::boardSize> genStaticMoves(const std::array<int, N> &relativeMoves, const std::array<uint64_t, N> &notBoardMaps)
    {
        constexpr std::array<uint64_t, Board::boardSize> moves = {};
        moves.fill(1);

        for (size_t i = 0; i < Board::boardSize; ++i)
        {
            moves[i] <<= i;
            for (size_t j = 0; i < N; ++i)
            {
                moves[i] |= (shift(moves[i], relativeMoves[j]) & notBoardMaps[j]);
            }
        }

        return moves;
    }

    //----------------------------------------------------------------------------------------------

    /*
    2D Array containing hard-coded bitbards with bits set that are between (vertical, horizoontal, diagonal) from-to indicies

    * For the not hirozontal, vertical or diagonal squares array value is set to zeros bit board
    */
    inline static constexpr std::array<std::array<uint64_t, Board::boardSize>, Board::boardSize> inBetween = [] () constexpr
    {
        std::array<std::array<uint64_t, Board::boardSize>, Board::boardSize> tab = {};

        for (size_t i = 0; i < Board::boardSize; ++i)
        {
            for (size_t j = 0; j < Board::boardSize; ++j)
            {
                if (i == j)
                {
                    continue;
                }
                int min = (i < j ? i : j);
                int max = (i > j ? i : j);
                if (i / 8 == j / 8)
                {
                    tab[i][j] = static_cast<uint64_t>((1ULL << (std::abs(static_cast<int>(i) - static_cast<int>(j)) - 1)) - 1) << (min + 1);
                }
                else if (i % 8 == j % 8)
                {
                    for (int it = min+8; it < max; it+=8) 
                    {
                        tab[i][j] |= static_cast<uint64_t>(1) << it;
                    }
                }
                else if (std::abs(static_cast<int>(i)/8 - static_cast<int>(j)/8) == std::abs(static_cast<int>(i)%8 - static_cast<int>(j)%8))
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

    struct Slider {
        //------------------
        // Initializators
        //------------------

        Slider() = delete;
            
        //------------------
        // Methods
        //------------------
        
        // computes one of occupanices combination based on occupancie index - max combinations=4096
        // * param[in] : bits -> occupaniec number - max=12
        // * param[in] : mask -> occupancies mask
        /* chessprogramming */
        static constexpr uint64_t indexToUint64(const int index, const int bits, uint64_t mask)
        {
            uint64_t result = 0ULL;
            for(int i = 0; i < bits; i++)
            {
                int j = pop_1st(&mask);
                if (index & (1 << i))
                {
                    result |= (1ULL << j);
                }
            }
            return result;
        }

        static constexpr int transform(const uint64_t occupancieMask, const uint64_t magic, const int bits)
        {
            return (int)((mask * magic) >> (64 - bits));
        }
        static constexpr int transform(const uint64_t occupancieMask, const std::pair<uint64_t, int> magic)
        {
            return (int)((mask * magic.first) >> (64 - magic.second));
        }
    };

};



#endif
