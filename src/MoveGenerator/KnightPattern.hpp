#ifndef KNIGHT_PATTERN_HPP
#define KNIGHT_PATTERN_HPP

#include <cstdint>
#include <array>
#include "MoveUtils.hpp"


// Knight Piece, same as King Piece will be using predefined moves table to generate pseudo-legal moves


class KnightPattern {
    public:
    static constexpr uint8_t maxPosPosibilities = 8;

    //--------------
    // API function
    //--------------

    static constexpr uint64_t getMoves() {  } 

    private:
    static constexpr std::array<uint8_t, maxPosPosibilities> relativeMoves = { 17, 10, 6, 15, -15, -6, -10, -17 }

    static constexpr std::array<uint64_t, maxPosPosibilities> notBoardMaps = {  }
};




#endif
