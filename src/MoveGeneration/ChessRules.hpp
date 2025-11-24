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
#define CHESS_RULES_HPP

#include <cstdint>
#include <utility>
#include "Board.hpp"
#include "Move.hpp"
#include "BitOperation.hpp"
#include "MoveUtils.hpp"
#include "Perft/PerftStats.h"

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
    static constexpr std::array<uint64_t,2> KRight   { /* W */ 1ull<<3, /* B */ 1ull<<1 };
    static constexpr std::array<uint64_t,2> QRight   { /* W */ 1ull<<2, /* B */ 1ull<<0 };
    static constexpr std::array<uint64_t,2> KBlockers{ /* W */ (1ull<<5)|(1ull<<6), /* B */ (1ull<<61)|(1ull<<62) };
    static constexpr std::array<uint64_t,2> QBlockers{ /* W */ (1ull<<1)|(1ull<<2)|(1ull<<3), /* B */ (1ull<<57)|(1ull<<58)|(1ull<<59) };
    static constexpr std::array<uint64_t,2> KDest    { /* W */ (1ull<<6), /* B */ (1ull<<62) };
    static constexpr std::array<uint64_t,2> QDest    { /* W */ (1ull<<2), /* B */ (1ull<<58) };
    // ----------------------
    // Pawn helpers bit masks
    // ----------------------
    static constexpr std::array<uint64_t, 2> PawnDblPushOriginRow { 0xFFULL << 8, 0xFFULL << (6*8) };

    // --------------------
    // Initializators
    // --------------------

    ChessRules(Board &board, PerftStats &perft_stats)
        : _board{board}, _perft_stats{perft_stats} {}

    // --------------------
    // Methods
    // --------------------

    [[nodiscard]] Move* getLegalMoves(int originSq, Move *moves) const;
        
    bool isHalfMoveClkExtended() const { return _board.halfMoveClock >= 50; }
    // TODO: [Urgent] IMPLEMENT THREEFOLD RULE CHECK !

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
    [[nodiscard]] uint64_t xrayAttacks(int sq, uint64_t bbUs, uint64_t bbThem) const
    {
        uint64_t attacks = Attacks(sq, bbThem, bbUs);
        uint64_t blockers = bbThem & attacks;

        // retuns the attacks behind the pin when there was a pinner
        return attacks ^ Attacks(sq, bbUs, bbThem ^ blockers);
    }

    template <uint64_t (*Attacks)(const int, const uint64_t, const uint64_t), Sliders slider>
    [[nodiscard]] uint64_t getPins(/*kingSq*/int sq) const
    {
        const uint64_t sliderOpp = static_cast<bool>(slider) ? _board.bbThemRQ() : _board.bbThemBQ();
        uint64_t pinned = 0;
        uint64_t pinner = xrayAttacks<Attacks>(sq, _board.bbUs(), _board.bbThem()) & sliderOpp;
        while (pinner)
        {
            int sqP = pop_1st(pinner);
            pinned |= MoveUtils::inBetween[sqP][sq] & _board.bbUs();
        }

        return pinned;
    }

    [[nodiscard]] uint64_t getAllPins(int sq) const;

    // ---------------------------
    // Move Type Helpers
    // ---------------------------

    // King - Knight - Sliders (Rook, Bishop, Queen) -------------
    [[nodiscard]] const MoveType encodeIsCapture(int targetSq) const;

    // King Castling -------------
    [[nodiscard]] const MoveType encodeCastling(int targetSq) const;

    // Pawn ----------------------
    [[nodiscard]] const MoveType encodeIsDblPush(int originSq, int targetSq) const;

    [[nodiscard]] const MoveType encodeIsEp(int originSq, int targetSq) const;

    [[nodiscard]] const MoveType encodeIsPromotion(int originSq, int targetSq) const;

    // ---------------------------
    // King checks
    // ---------------------------

    [[nodiscard]] uint64_t getCastlingMoves() const;

    [[nodiscard]] const bool isCheck() const { return isAttackedTo(std::countr_zero(_board.bbUs(Piece::King)), _board.sideToMove); }

    [[nodiscard]] const bool isDoubleCheck() const 
    {
        uint64_t attackers = attacksTo(std::countr_zero(_board.bbUs(Piece::King)), _board.sideToMove);
        pop_1st(attackers);
        return static_cast<bool>(attackers);
    }

    // return: first: King atackers, second: Evasion paths -> e.g. inBetween Rook -> King square
    [[nodiscard]] std::pair<uint64_t, uint64_t> getEvasions() const;

    Board &_board;
    PerftStats &_perft_stats;
};


#endif
