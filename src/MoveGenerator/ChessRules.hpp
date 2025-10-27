// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Class for cheking moves legality/pseudo legality
/*************************************************/
// All piece clear pseudo-legal moves are generated
// in particural piece class.
// "clear pseudo-legal mvoe" - pseudo legal move
// without en passant captures, castling, ...

#ifndef CHESS_RULES_HPP
#define ChESS_RULES_HPP

#include <cstdint>
#include <utility>
#include "Board.hpp"
#include "Move.hpp"

#define MAX_MOVES_NUMBER    (256)   // rael max is 218

// Stateless class, only pure chess mechanics
class ChessRules
{
public:
    // --------------------
    // Castling bit masks
    // --------------------
    constexpr std::array<uint64_t,2> KRight   { /* W */ 1ull<<3, /* B */ 1ull<<1 };
    constexpr std::array<uint64_t,2> QRight   { /* W */ 1ull<<2, /* B */ 1ull<<0 };
    constexpr std::array<uint64_t,2> KBlockers{ /* W */ (1ull<<5)|(1ull<<6), /* B */ (1ull<<61)|(1ull<<62) };
    constexpr std::array<uint64_t,2> QBlockers{ /* W */ (1ull<<1)|(1ull<<2)|(1ull<<3), /* B */ (1ull<<57)|(1ull<<58)|(1ull<<59) };
    constexpr std::array<uint64_t,2> KDest    { /* W */ (1ull<<6), /* B */ (1ull<<62) };
    constexpr std::array<uint64_t,2> QDest    { /* W */ (1ull<<2), /* B */ (1ull<<58) };


    // --------------------
    // Initializators
    // --------------------
    ChessRules(Board &board)
        : _board{board} {}

    // --------------------
    // Methods
    // --------------------
    // without cheking is the King attacked
    [[nodiscard]] const uint64_t getCastlingMoves() const;
        
private:
    Board &_board;

    // --------------------
    // Methods - helpers
    // --------------------
    [[nodiscard]] const uint64_t attacksTo(const int sq, const pColor movePColor) const;
};

#endif
