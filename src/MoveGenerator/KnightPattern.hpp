#ifndef KNIGHT_PATTERN_HPP
#define KNIGHT_PATTERN_HPP

#include <cstdint>
#include <array>
#include "MoveUtils.hpp"


// Knight Piece, same as King Piece will be using predefined moves table to generate pseudo-legal moves


class KnightPattern 
{
public:
    static constexpr uint8_t maxPosPosibilities = 8;

    //--------------
    // API function
    //--------------

    static constexpr uint64_t getMoves(const uint64_t bbUs) 
    { 
        MoveUtils::genStaticMoves(relativeMoves, notBoardMaps) & ~bbUs; 
    }

private:
    static constexpr std::array<int, maxPosPosibilities> relativeMoves = { 6, 15, 17, 10, -6, -15, -17, -10 }
    static constexpr std::array<uint64_t, maxPosPosibilities> notBoardMaps = { notGHFile, notHFile, notAFile, notABFile, notABFile, notAFile, notHFile, notGHFile }

    static constexpr uint64_t notAFile = 0x7F7F7F7F7F7F7F7F;
    static constexpr uint64_t notABFile = 0x3F3F3F3F3F3F3F3F;
    static constexpr uint64_t notHFile = 0xFEFEFEFEFEFEFEFE;
    static constexpr uint64_t notGHFile = 0xFCFCFCFCFCFCFCFC;
};




#endif
