// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Class for moves generation - used by ChessRules, which is considered as Higher Level Interface
/*************************************************/

#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include "Move.hpp"
#include "Board.hpp"
#include "MoveEncoder.h"

#include <utility>


enum class Gen : size_t
{
    Captures,
    Quiets,
    Evasions,
    All     // -> Captures + Quiets
}

template<Gen G>
struct GenTraits;

template<> struct GenTraits<Gen::Captures> { static constexpr bool Captures = true;  static constexpr bool Quiets = false; static constexpr bool Evasions = false; };
template<> struct GenTraits<Gen::Quiets>   { static constexpr bool Captures = false; static constexpr bool Quiets = true;  static constexpr bool Evasions = false; };
template<> struct GenTraits<Gen::Evasions>{ static constexpr bool Captures = false; static constexpr bool Quiets = false; static constexpr bool Evasions = true;  };
template<> struct GenTraits<Gen::All>     { static constexpr bool Captures = true;  static constexpr bool Quiets = true;  static constexpr bool Evasions = false; };


class MoveGen
{
public:
    // -------------------------
    // Main API move generation.
    // -------------------------

    template<Gen GenMode>
    [[nodiscard]] Move* generate(Board &_board, Move *moves);

private:
    template<typename EncodeFn, typename AllowFn>
    [[nodiscard]] addTargetsAsMove(uint64_t targets, int originSq, Move *moves, EncodeFn encode, AllowFn allow)
    {
        while (targets)
        {
            int sq = pop_1st(targets);
            if ( allow(sq) )
            {
                *moves++ = Move(originSq, sq, encode());
            }
        }

        return moves;
    }

    // -------------------------
    // Piece Types Helpers
    // -------------------------

    template<Gen G>
    [[nodiscard]] Move* getKingMoves(Board &_board, Move *moves);

    template<Gen G>
    [[nodiscard]] Move* getKnightMoves(Board &_board, Move *moves);

    template<Gen G>
    [[nodiscard]] Move* getBishopMoves(Board &_board, Move *moves);

        template<Gen G>
    [[nodiscard]] Move* getKnightMoves(Board &_board, Move *moves);

        template<Gen G>
    [[nodiscard]] Move* getKnightMoves(Board &_board, Move *moves);

    // -------------------------
    // Gen Types Helpers
    // -------------------------

    [[nodiscard]] Move* genCaptures(Board &_board, Move *moves);

    [[nodiscard]] Move* genQuiets(Board &_board, Move *moves);

    [[nodiscard]] Move* genEvasions(Board &_board, Move *moves);

    [[nodiscard]] Move* genAll(Board &_board, Move *moves);
};

#endif  // MOVE_GENERATOR_H


// ------------------------------
// INLINE (TEMPLATES) DEFINITIONS
// ------------------------------

template<Gen GenMode>
[[nodiscard]] Move* MoveGen::generate(Board &_board, Move *moves)
{
    if constexpr (GenTraits<GenMode>::Captures)
    {
        return genCaptures(Board &_board, Move *moves);
    }
    
    if constexpr (GenTraits<GenMode>::Quiets)
    {
        return genQuiets(Board &_board, Move *moves);
    }

    if constexpr (GenTraits<GenMode>::Evasions)
    {
        return genEvasions(Board &_board, Move *moves);
    }

    // All case already handled

    std::unreachable();
    return moves;
}

// -------------------------
// Piece Types Helpers
// -------------------------

template<Gen G>
[[nodiscard]] Move* getKingMoves(Board &_board, Move *moves)
{
    const int kingSq = count_1s(_board.bbUs(Piece::King));
    const uint64_t moves = KingPattern::getMoves(static_cast<size_t>(kingSq), _board.bbUs());

    if constexpr (GenTraits<G>::Captures)
    {
        uint64_t attacks = moves & _board.bbThem();         

        moves = addTargetsAsMove(attacks, kingSq, moves, [] () { return MoveType::CAPTURE; }, [] (int sq) { return !isAttackedTo(sq); });
    }

    if constexpr (GenTraits<G>::Quiets)
    {
        uint64_t quiets = moves & ~_board.bbThem();         

        moves = addTargetsAsMove(quiets, kingSq, moves, [] () { return MoveType::QUIETS; }, [] (int sq) { return !isAttackedTo(sq); });

        // add castling moves
        uint64_t castlings = getCastlingMoves();
        moves = addTargetsAsMove(castlings, kingSq, moves, [] (int sq) { return MoveEncoder::encodeCastling(_board, sq); }, [] (int) { return true; });
    }

    if constexpr (GenTraits<G>::Evasions)
    {
        // TODO
    }

    return moves;
}

template<Gen G>
[[nodiscard]] Move* getKnightMoves(Board &_board, Move *moves)
{
    uint64_t Knights = _board.bbUs(Piece::Knight);
    while (Knights)
    {
        int knightSq = pop_1st(Knights);
        const uint64_t moves = KingPattern::getMoves(static_cast<size_t>(knightSq), _board.bbUs());

        if constexpr (GenTraits<G>::Captures)
        {
            uint64_t attacks = moves & _board.bbThem();         

            moves = addTargetsAsMove(attacks, knightSq, moves, [] () { return MoveType::CAPTURE; }, [] (int sq) { return true; });
        }

        if constexpr (GenTraits<G>::Quiets)
        {
            uint64_t quiets = moves & ~_board.bbThem();         

            moves = addTargetsAsMove(quiets, knightSq, moves, [] () { return MoveType::QUIETS; }, [] (int sq) { return true; });
        }

        if constexpr (GenTraits<G>::Evasions)
        {
            // TODO
        }
    }

    return moves;
}

template<Gen G>
[[nodiscard]] Move* getBishopMoves(Board &_board, Move *moves)
{
    uint64_t Knights = _board.bbUs(Piece::Knight);
    while (Knights)
    {
        int knightSq = pop_1st(Knights);
        const uint64_t moves = KingPattern::getMoves(static_cast<size_t>(knightSq), _board.bbUs());

        if constexpr (GenTraits<G>::Captures)
        {
            uint64_t attacks = moves & _board.bbThem();         

            moves = addTargetsAsMove(attacks, knightSq, moves, [] () { return MoveType::CAPTURE; }, [] (int sq) { return true; });
        }

        if constexpr (GenTraits<G>::Quiets)
        {
            uint64_t quiets = moves & ~_board.bbThem();         

            moves = addTargetsAsMove(quiets, knightSq, moves, [] () { return MoveType::QUIETS; }, [] (int sq) { return true; });
        }

        if constexpr (GenTraits<G>::Evasions)
        {
            // TODO
        }
    }

    return moves;
}
