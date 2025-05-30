#ifndef KING_PATTERN
#define KING_PATTERN

#include <cstdint>
#include <array>
#include "MoveUtils.hpp"

// TODO
// Consider as in White/Black Pawn Maps the abstraction with KnightPattern


class KingPattern {
    public:
    static constexpr uint8_t maxPosPosibilities = 8;

    //--------------
    // API function
    //--------------

    static constexpr uint64_t getMoves() { MoveUtils::genStaticMoves(relativeMoves, notBoardMaps); } 

    private:
    static constexpr std::array<int, maxPosPosibilities> relativeMoves = { 6, 15, 17, 10, -6, -15, -17, -10 }
    static constexpr std::array<uint64_t, maxPosPosibilities> notBoardMaps = { notGHFile, notHFile, notAFile, notABFile, notABFile, notAFile, notHFile, notGHFile }

    static constexpr uint64_t notAFile = 0x7F7F7F7F7F7F7F7F;
    static constexpr uint64_t notABFile = 0x3F3F3F3F3F3F3F3F;
    static constexpr uint64_t notHFile = 0xFEFEFEFEFEFEFEFE;  
    static constexpr uint64_t notGHFile = 0xFCFCFCFCFCFCFCFC;
};





#endif
