#ifndef KING_PATTERN
#define KING_PATTERN

#include <cstdint>
#include <array>
#include "MoveUtils.hpp"

// TODO
// 1. Consider to implement here the tagets to move -> moves & ~taboo, taboo squares = possible to move for king
// 2. Consider as in White/Black Pawn Maps the abstraction with KnightPattern



class KingPattern {
    public:
    static constexpr uint8_t maxPosPosibilities = 8;

    //--------------
    // API function
    //--------------

    static constexpr uint64_t getMoves() { MoveUtils::genStaticMoves(relativeMoves, notBoardMaps); } 

    private:
    // next 2 arrays have to be initialized by order accordingly each other
    static constexpr std::array<int, maxPosPosibilities> relativeMoves = { 1, 7, 8, 9, -1, -7, -8, -9 };
    static constexpr std::array<uint64_t, maxPosPosibilities> notBoardMaps = { notHFile, notHile, notAny, notAFile, notAFile, notAFile, notAny, notHFile };

    static constexpr uint64_t notAFile = 0x7F7F7F7F7F7F7F7F;
    static constexpr uint64_t notHFile = 0xFEFEFEFEFEFEFEFE;
    static constexpr uint64_t notAny =   0xFFFFFFFFFFFFFFFF;
};



#endif
