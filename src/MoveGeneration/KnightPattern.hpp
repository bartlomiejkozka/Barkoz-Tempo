#ifndef KNIGHT_PATTERN_HPP
#define KNIGHT_PATTERN_HPP

#include <cstdint>
#include <array>

#include "MoveUtils.hpp"
#include "Board.hpp"


// Knight Piece, same as King Piece will be using predefined moves table to generate pseudo-legal moves


struct KnightPattern 
{
    static constexpr uint8_t maxPosPosibilities = 8;
    static constexpr uint64_t notAFile = 0x7F7F7F7F7F7F7F7F;
    static constexpr uint64_t notABFile = 0x3F3F3F3F3F3F3F3F;
    static constexpr uint64_t notHFile = 0xFEFEFEFEFEFEFEFE;
    static constexpr uint64_t notGHFile = 0xFCFCFCFCFCFCFCFC;

private:
    static constexpr std::array<int, maxPosPosibilities> relativeMoves = { 6, 15, 17, 10, -6, -15, -17, -10 }
    static constexpr std::array<uint64_t, maxPosPosibilities> notBoardMaps = { notGHFile, notHFile, notAFile, notABFile, notABFile, notAFile, notHFile, notGHFile }

public:
    //---------------------
    // API function - Moves
    //---------------------

    static const uint64_t getMoves(const size_t originSq, const uint64_t bbUs) 
    { 
        MoveUtils::genStaticMoves<maxPosPosibilities>(relativeMoves, notBoardMaps)[originSq] & ~bbUs;
    }

    //----------------------
    // API table - AttacksTo
    //----------------------

    static constexpr std::array<uint64_t, Board::boardSize> attacksTo =
        MoveUtils::genStaticMoves<maxPosPosibilities>(relativeMoves, notBoardMaps);
};




#endif
