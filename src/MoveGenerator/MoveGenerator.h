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
#include "KingPattern.hpp"
#include "KnightPattern.hpp"
#include "BishopMap.h"
#include "RookMap.h"
#include "QueenMap.h"
#include "BlackPawnMap.hpp"
#include "WhitePawnMap.hpp"
#include "ChessRules.hpp"

#include <utility>
#include <array>
#include <type_traits>


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

    [[nodiscard]] static Move* generateLegalMoves(const ChessRules &rules, Move *moves);

    template<Gen GenMode>
    [[nodiscard]] static Move* generate(const ChessRules &rules, Move *moves);

private:
    template<typename EncodeFn, typename AllowFn>
    [[nodiscard]] static addTargetsAsMove(uint64_t targets, int originSq, Move *moves, EncodeFn encode, AllowFn allow);

    template<Gen G,
         typename GetMovesFn,   
         typename EncodeFn,     
         typename AllowFn,    
         typename PostFn = std::nullptr_t>
    [[nodiscard]] static Move* generatePieceMoves(ChessRules &rules,
                            Move* moves,
                            uint64_t piecesBB,
                            GetMovesFn getMoves,
                            AllowFn allow,
                            PostFn post = nullptr);

    // -------------------------
    // Piece Types Helpers
    // -------------------------

    template<Gen G>
    [[nodiscard]] static Move* getKingMoves(ChessRules &rules, Move *moves);

    template<Gen G>
    [[nodiscard]] static Move* getKnightMoves(ChessRules &rules, Move *moves);

    template<Gen G>
    [[nodiscard]] static Move* getBishopMoves(ChessRules &rules, Move *moves);

    template<Gen G>
    [[nodiscard]] static Move* getRookMoves(ChessRules &rules, Move *moves);

    template<Gen G>
    [[nodiscard]] static Move* getQueenMoves(ChessRules &rules, Move *moves);

    template<Gen G>
    [[nodiscard]] static Move* getPawnMoves(ChessRules &rules, Move *moves);
};

#endif  // MOVE_GENERATOR_H


// ------------------------------
// INLINE (TEMPLATES) DEFINITIONS
// ------------------------------

[[nodiscard]] static Move* generateLegalMoves(const ChessRules &rules, Move *moves);
{
    if ( !rules.isCheck() )
    {
        moves = generate<Gen::All>(rules, moves);
    }
    else
    {
        if ( !rules.isDoubleCheck() )
        {
            moves = generate<Gen::Evasions>(rules, getKingMoves<Gen::All>(rules, moves));
        }
        // Double Check -> only King evasion
        else
        {
            moves = getKingMoves<Gen::All>(rules, moves);
        }
    }
    
    return moves;
}

template<Gen GenMode>
[[nodiscard]] static Move* MoveGen::generate(const ChessRules &rules, Move *moves)
{
    std::array<Move*(*)(Board&, Move*), 6> getMoves = 
    { 
        getKingMoves<GenMode>, getKnightMoves<GenMode>, getPawnMoves<GenMode>,
        getBishopMoves<GenMode>, getRookMoves<GenMode>, getQueenMoves<GenMode> 
    };

    for (auto &getMove : getMoves)
    {
        moves = getMove(rules._board, moves);
    }

    return moves;
}

// -------------------------
// Piece Types Gen Template
// -------------------------

template<typename EncodeFn, typename AllowFn>
[[nodiscard]] static Move* MoveGen::addTargetsAsMove(uint64_t targets, int originSq, Move *moves, EncodeFn encode, AllowFn allow)
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

template<Gen G, Piece P
         typename GetMovesFn,   
         typename EncodeFn,     
         typename AllowFn,    
         typename PostFn = std::nullptr_t>
[[nodiscard]] static Move* MoveGen::generatePieceMoves(ChessRules &rules,
                         Move* moves,
                         GetMovesFn getMoves,
                         AllowFn allow,
                         PostFn post = nullptr)
{
    uint64_t piecesBB = rules._board.bbUs(P);

    while (piecesBB)
    {
        int fromSq = pop_1st(piecesBB);
        uint64_t targets = getMoves(fromSq);

        if constexpr (GenTraits<G>::Captures)
        {
            uint64_t captures = targets & board.bbThem();
            moves = addTargetsAsMove(captures, fromSq, moves, [](){ return return MoveType::CAPTURE; }, [](int sq){ return allow(sq); });
        }

        if constexpr (GenTraits<G>::Quiets)
        {
            uint64_t quiets = targets & ~board.bbThem();
            moves = addTargetsAsMove(quiets, fromSq, moves, [](){ return MoveType::QUIET; }, [](int sq){ return allow(sq); });
        }

        if constexpr (GenTraits<G>::Evasions)
        {
            std::pair<uint64_t, uint64_t> AttackerAndEvasionPath = rules.getEvasions();    // in case of single check, there is only one King Attacker, in case of double check -> should condider only King evasion

            // capture
            uint64_t capture = targets & board.bbThem() & AttackerAndEvasionPath.first;   
            *moves++ = Move(fromSq, capture, MoveType::CAPTURE);

            // covers King
            if (AttackerAndEvasionPath.second)
            {
                uint64_t evasions = targets & ~board.bbThem() & AttackerAndEvasionPath.second;
                moves = addTargetsAsMove(evasions, fromSq, moves, [](){ return return MoveType::QUIET; }, [](int sq){ return allow(sq); });
            }
        }

        // optional post-processing hook (e.g., castling, pawn double push)
        if constexpr (!std::is_same_v<PostFn, std::nullptr_t>)
        {
            moves = post(fromSq, targetSq);
        }
    }

    return moves;
}



// -------------------------
// Piece Types Helpers
// -------------------------

template<Gen G>
[[nodiscard]] static Move* getKingMoves(ChessRules &rules, Move *moves)
{
    return generatePieceMoves<G, Piece::King>
    (
        rules._board,
        moves,
        [&] (int fromSq) { return KingPattern::getMoves(static_cast<size_t>(fromSq), rules._board.bbUs()); },
        [] (int sq) { return !rules.isAttackedTo(sq); },
        // post -> add castling moves to quiet moves
        [&] (int fromSq, int targetSq) 
        {
            if constexpr ( GenTraits<G>::Quiets )
            {
                // TODO: check if we check is the King in check in case getting castling moves
                uint64_t castlings = rules.getCastlingMoves();
                return addTargetsAsMove(castlings, fromSq, moves, [](){ MoveEncoder::encodeCastling(rules._board, targetSq) }, [](){ return true; });
            }
        }
    );
}

template<Gen G>
[[nodiscard]] static Move* getKnightMoves(ChessRules &rules, Move *moves)
{
    return generatePieceMoves<G, Piece::Knight>
    (
        rules._board,
        moves,
        [&] (int fromSq) { return KnightPattern::getMoves(static_cast<size_t>(fromSq), rules._board.bbUs()); },
        [] (int) { return true; }
    );
}

template<Gen G>
[[nodiscard]] static Move* getBishopMoves(ChessRules &rules, Move *moves)
{
    return generatePieceMoves<G, Piece::Bishop>
    (
        rules._board,
        moves,
        [&] (int fromSq) { return BishopMap::getMoves(static_cast<size_t>(fromSq),rules._board.bbUs()); },
        [] (int) { return true; }
    );
}

template<Gen G>
[[nodiscard]] static Move* getRookMoves(ChessRules &rules, Move *moves)
{
    return generatePieceMoves<G, Piece::Rook>
    (
        rules._board,
        moves,
        [&] (int fromSq) { return RookMap::getMoves(static_cast<size_t>(fromSq), rules._board.bbUs()); },
        [] (int) { return true; }
    );
}

template<Gen G>
[[nodiscard]] static Move* getQueenMoves(ChessRules &rules, Move *moves)
{
    return generatePieceMoves<G, Piece::Queen>
    (
        rules._board,
        moves,
        [&] (int fromSq) { return QueenMap::getMoves(static_cast<size_t>(fromSq), rules._board.bbUs()); },
        [] (int) { return true; }
    );
}

template<Gen G>
[[nodiscard]] static Move* getPawnMoves(ChessRules &rules, Move *moves)
{
    return generatePieceMoves<G, Piece::Pawn>
    (
        rules._board,
        moves,
        [&] (int fromSq) 
        { 
            return rules._board.sideToMove ? BlackPawnMap::getPushTargets(fromSq, rules._board.fullBoard()) :  WhitePawnMap::getPushTargets(fromSq, rules._board.fullBoard());
        },
        [] (int) { return true; }
        // post -> add Dbl Pushes & Ep attacks
        [&] (fromSq, targetSq)
        {
            if constexpr ( GenTraits<G>::Quiets )
            {
                return addTargetsAsMove(dblPushes, fromSq, moves, [](){ return return MoveType::DOUBLE_PUSH; }, [](){ return true; });
                uint64_t dblPushes = rules._board.sideToMove ? BlackPawnMap::getDblPushTargets(fromSq, rules._board.fullBoard()) :  WhitePawnMap::getDblPushTargets(fromSq, rules._board.fullBoard());
            }

            if constexpr ( GenTraits<G>::Captures )
            {
                uint64_t epAttack = rules._board.sideToMove ? BlackPawnMap::getEpAttackTarget(fromSq, rules._board.enPassant) :  getEpAttackTarget(fromSq, rules._board.enPassant);
                return addTargetsAsMove(epAttack, fromSq, moves, [](){ return return MoveType::EP_CAPTURE; }, [](){ return true; });
            }
        }
    );
}
