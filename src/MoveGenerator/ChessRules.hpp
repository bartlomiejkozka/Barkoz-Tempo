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
#include "BitOperation.hpp"
#include "MoveUtils.hpp"

#define MAX_MOVES_NUMBER    (256)   // rael max is 218

// Stateless class, only pure chess mechanics
class ChessRules
{
public:
    enum class Sliders
    {
        Bishop,
        Rook
    };

    static constexpr int MovesBufferSize = 256; // the max number of moves in single move is 218

    static constexpr int castlingMaxPossibilities = 4;
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

    [[nodiscard]] uint64_t attacksTo(const int sq, const pColor movePColor) const;

    [[nodiscard]] const bool isAttackedTo(const int sq, const pColor movePColor) const;

    [[nodiscard]] const bool isPathSafe(uint64_t pathSq, const pColor movePColor) const;

    // --------------------
    // Pins (x-rays)
    // --------------------

    // blockers: by default it is attacked pieces bitboard
    // use example to get pinners: xrayAttacks() & bbThem;
    template <uint64_t (*Attacks)(const int, const uint64_t, const uint64_t)>
    [[nodiscard]] uint64_t ChessRules::xrayAttacks(int sq, uint64_t bbUs, uint64_t bbThem) const;

    template <uint64_t (*Attacks)(const int, const uint64_t, const uint64_t), Sliders slider>
    [[nodiscard]] uint64_t getPins(/*kingSq*/int sq) const;

    [[nodiscard]] uint64_t getAllPins(int sq) const;

    // ---------------------------
    // Move Type Helpers
    // ---------------------------

    // King - Knight -------------
    [[nodiscard]] const MoveType encodeKingKnightMoveTypePure(int targetSq);

    // King Castling -------------
    [[nodiscard]] const MoveType encodeCastling(int targetSq);

    // ---------------------------
    // King Move
    // ---------------------------

    // Regular legal king moves + castlings 
    uint64_t getKingMove() const;
};


// ---------------------
// Functions definitions
// ---------------------

template <uint64_t (*Attacks)(const int, const uint64_t, const uint64_t)>
[[nodiscard]] uint64_t ChessRules::xrayAttacks(int sq, uint64_t bbUs, uint64_t bbThem) const
{
    uint64_t attacks = Attacks(sq, bbThem, bbUs);
    uint64_t blockers = bbThem & attacks;

    // retuns the attacks behind the pin when there was a pinner
    return attacks ^ Attacks(sq, bbUs, bbThem ^ blockers);
}

template <uint64_t (*Attacks)(const int, const uint64_t, const uint64_t), Sliders slider>
[[nodiscard]] uint64_t ChessRules::getPins(/*kingSq*/int sq) const
{
    const uint64_t sliderOpp = slider ? _board.bbThemRQ() : _board.bbUsBQ();
    uint64_t pinned = 0;
    pinner = xrayAttacks<Attacks>(sq, _board.bbUs(), _board.bbThem()) & sliderOpp;
    while (pinner)
    {
        int sqP = pop_1st(pinner);
        pinned |= inBetween[sqP][sq] & _board.bbUs();
    }

    return pinned;
}

#endif
