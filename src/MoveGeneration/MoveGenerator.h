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
#include "MoveUtils.hpp"
#include "BitOperation.hpp"

#include <utility>
#include <array>
#include <type_traits>
#include <bit>


enum class Gen : size_t
{
    Captures,
    Quiets,
    Evasions,
    All     // -> Captures + Quiets
};

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

    [[nodiscard]] static int generateLegalMoves(ChessRules &rules, Move *moves, int *outMobilityScore = nullptr, const int *mobilityWeights = nullptr);

    template<Gen GenMode>
    [[nodiscard]] static Move* generate(ChessRules &rules, Move *moves, int *outMobilityScore = nullptr, const int *mobilityWeights = nullptr);

private:
    template<typename EncodeFn, typename AllowFn>
    [[nodiscard]] static Move* addTargetsAsMove(uint64_t targets, int originSq, Move *moves, EncodeFn encode, AllowFn allow, bool isPromotion);

    template<Gen G, Piece P,
         typename GetMovesFn,     
         typename AllowFn,    
         typename PostFn = std::nullptr_t>
    [[nodiscard]] static Move* generatePieceMoves(ChessRules &rules,
                            Move* moves,
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

template<Gen GenMode>
[[nodiscard]] Move* MoveGen::generate(ChessRules &rules, Move *moves, int *outMobilityScore, const int *mobilityWeights)
{
    std::array<Move*(*)(ChessRules&, Move*), 6> getMoves = 
    { 
        getKingMoves<GenMode>, getKnightMoves<GenMode>, getPawnMoves<GenMode>,
        getBishopMoves<GenMode>, getRookMoves<GenMode>, getQueenMoves<GenMode> 
    };

for (int i = 0; i < 6; ++i)
    {
        Move* startPtr = moves;
        
        moves = getMoves[i](rules, moves);

        if (outMobilityScore != nullptr && mobilityWeights != nullptr)
        {
            auto count = static_cast<int>(moves - startPtr);
            *outMobilityScore += count * mobilityWeights[i];
        }
    }

    return moves;
}

// -------------------------
// Piece Types Gen Template
// -------------------------

template<typename EncodeFn, typename AllowFn>
[[nodiscard]] Move* MoveGen::addTargetsAsMove(uint64_t targets, int originSq, Move *moves, EncodeFn encode, AllowFn allow, bool isPromotion)
{   
    while (targets)
    {
        int sq = pop_1st(targets);
        if ( allow(sq, originSq) )
        {
            if ( isPromotion )
            {
                MoveType mtBase = (encode(sq) == MoveType::QUIET) ? MoveType::Q_PROM : MoveType::Q_PROM_CAP;
                for (int i = 0; i < 4; ++i)
                {
                    MoveType mt = static_cast<MoveType>(static_cast<int>(mtBase) - i);
                    *moves++ = Move(originSq, sq, mt);
                }
            }
            else
            {
                Move move = Move(originSq, sq, encode(sq));
                *moves++ = move;
            }
        }
    }

    return moves;
}

template<Gen G, Piece P,
         typename GetMovesFn,      
         typename AllowFn,    
         typename PostFn>
[[nodiscard]] Move* MoveGen::generatePieceMoves(ChessRules &rules,
                         Move* moves,
                         GetMovesFn getMoves,
                         AllowFn allow,
                         PostFn post)
{
    uint64_t piecesBB = rules._board.bbUs(P);
    const int kingSq = std::countr_zero(rules._board.bbUs(Piece::King));
    uint64_t pinned;
    std::pair<uint64_t, uint64_t> AttackerAndEvasionPath{0,0};
    bool isPromotion = false;

    pinned = rules.getAllPins(kingSq);
    // if ( pinned ) rules._perft_stats.discovery_checks++;

    while (piecesBB)
    {
        int fromSq = pop_1st(piecesBB);
        uint64_t targets = getMoves(fromSq);

        if constexpr ( P == Piece::Pawn )
        {
            isPromotion = rules.isBeforeLastRnak(fromSq);
        }

        if ( (minBitSet << fromSq) & pinned )
        {
            targets = rules.getNotPinnedTargets(targets, kingSq, fromSq, P);
        }

        if constexpr (GenTraits<G>::Captures)
        {
            uint64_t captures = targets & rules._board.bbThem();
            moves = addTargetsAsMove(captures, fromSq, moves, [](int){ return MoveType::CAPTURE; }, 
                [&](int sq, int originSq){ return allow(sq, originSq); }, isPromotion);
        }

        if constexpr (GenTraits<G>::Quiets)
        {
            uint64_t quiets = targets & ~rules._board.bbThem();
            moves = addTargetsAsMove(quiets, fromSq, moves, [](int){ return MoveType::QUIET; }, 
                [&](int sq, int originSq){ return allow(sq, originSq); }, isPromotion);
        }

        if constexpr (GenTraits<G>::Evasions)
        {
            if (fromSq != kingSq)
            {
                AttackerAndEvasionPath = rules.getEvasions();    // in case of single check, there is only one King Attacker, in case of double check -> should condider only King evasion

                // capture
                if (uint64_t capture = targets & rules._board.bbThem() & AttackerAndEvasionPath.first; 
                    capture)
                {
                    // *moves++ = Move(fromSq, std::countr_zero(capture), MoveType::CAPTURE);
                    moves = addTargetsAsMove(capture, fromSq, moves, [](int){ return MoveType::CAPTURE; }, 
                        [&](int sq, int originSq){ return allow(sq, originSq); }, isPromotion);
                }
                // covers King
                if (AttackerAndEvasionPath.second)
                {
                    uint64_t evasions = targets & ~rules._board.bbThem() & AttackerAndEvasionPath.second;
                    moves = addTargetsAsMove(evasions, fromSq, moves, [](int){ return MoveType::QUIET; }, 
                        [&](int sq, int originSq){ return allow(sq, originSq); }, isPromotion);
                }
            }
        }

        // optional post-processing hook (e.g., castling, pawn double push)
        if constexpr ( !std::is_same_v<PostFn, std::nullptr_t>)
        {
            moves = post(fromSq, moves, pinned, kingSq, AttackerAndEvasionPath);
        }
    }

    return moves;
}



// -------------------------
// Piece Types Helpers
// -------------------------

template<Gen G>
[[nodiscard]] Move* MoveGen::getKingMoves(ChessRules &rules, Move *moves)
{
    return generatePieceMoves<G, Piece::King>
    (
        rules,
        moves,
        [&rules] (int fromSq) { return KingPattern::getMoves(static_cast<size_t>(fromSq), rules._board.bbUs()); },
        [&rules] (int sq, int fromSq) { return !rules.isAttackedTo(sq, rules._board.sideToMove, rules._board.bbUs()^bitBoardSet(fromSq), rules._board.bbThem()); },
        // post -> add castling moves to quiet moves
        [&rules] (int fromSq, Move *moves, uint64_t, int, std::pair<uint64_t, uint64_t>) 
        {
            if constexpr ( GenTraits<G>::Quiets )
            {
                if ( !rules.isCheck() )
                {
                    uint64_t castlings = rules.getCastlingMoves();
                    return addTargetsAsMove(castlings, fromSq, moves, [&](int targetSq){ return MoveEncoder::encodeCastling(rules._board, targetSq); }, 
                        [](int, int){ return true; }, false);
                }
            }
            return moves;
        }
    );
}

template<Gen G>
[[nodiscard]] Move* MoveGen::getKnightMoves(ChessRules &rules, Move *moves)
{
return generatePieceMoves<G, Piece::Knight>
    (
        rules,
        moves,
        [&] (int fromSq) { return KnightPattern::getMoves(static_cast<size_t>(fromSq), rules._board.bbUs()); },
        [] (int, int) { return true; }
    );
}

template<Gen G>
[[nodiscard]] Move* MoveGen::getBishopMoves(ChessRules &rules, Move *moves)
{
    return generatePieceMoves<G, Piece::Bishop>
    (
        rules,
        moves,
        [&] (int fromSq) { return Bishop::getMoves(static_cast<size_t>(fromSq), rules._board.bbUs(), rules._board.bbThem()); },
        [] (int, int) { return true; }
    );
}

template<Gen G>
[[nodiscard]] Move* MoveGen::getRookMoves(ChessRules &rules, Move *moves)
{
    return generatePieceMoves<G, Piece::Rook>
    (
        rules,
        moves,
        [&] (int fromSq) { return Rook::getMoves(static_cast<size_t>(fromSq), rules._board.bbUs(), rules._board.bbThem()); },
        [] (int, int) { return true; }
    );
}

template<Gen G>
[[nodiscard]] Move* MoveGen::getQueenMoves(ChessRules &rules, Move *moves)
{
    return generatePieceMoves<G, Piece::Queen>
    (
        rules,
        moves,
        [&] (int fromSq) { return Queen::getMoves(static_cast<size_t>(fromSq), rules._board.bbUs(), rules._board.bbThem()); },
        [] (int, int) { return true; }
    );
}

template<Gen G>
[[nodiscard]] Move* MoveGen::getPawnMoves(ChessRules &rules, Move *moves)
{
    // TODO: pawn promotion handling
    return generatePieceMoves<G, Piece::Pawn>
    (
        rules,
        moves,
        [&rules] (int fromSq) 
        {   
            return static_cast<bool>(rules._board.sideToMove) ?
                       BlackPawnMap::getPushTargets(fromSq, rules._board.fullBoard()) | BlackPawnMap::getAnyAttackTargets(fromSq, rules._board.bbThem())
                    :  WhitePawnMap::getPushTargets(fromSq, rules._board.fullBoard()) | WhitePawnMap::getAnyAttackTargets(fromSq, rules._board.bbThem());    
        },
        [] (int, int) { return true; },
        // post -> add Dbl Pushes & Ep attacks
        [&rules] (int fromSq, Move *moves, uint64_t pinned, int kingSq, std::pair<uint64_t, uint64_t> AttackerAndEvasionPath)
        {
            if constexpr ( GenTraits<G>::Quiets )
            {
                uint64_t dblPushes = static_cast<bool>(rules._board.sideToMove) ? BlackPawnMap::getDblPushTargets(fromSq, rules._board.fullBoard()) :  WhitePawnMap::getDblPushTargets(fromSq, rules._board.fullBoard());
                if ( bitBoardSet(fromSq) & pinned )
                {
                    dblPushes = rules.getNotPinnedTargets(dblPushes, kingSq, fromSq, Piece::Pawn);
                }
                moves = addTargetsAsMove(dblPushes, fromSq, moves, [](int){ return MoveType::DOUBLE_PUSH; }, [](int, int){ return true; }, false);
            }

            if constexpr ( GenTraits<G>::Evasions )
            {
                if (AttackerAndEvasionPath.second)
                {
                    uint64_t dblPushes = static_cast<bool>(rules._board.sideToMove) ? BlackPawnMap::getDblPushTargets(fromSq, rules._board.fullBoard()) :  WhitePawnMap::getDblPushTargets(fromSq, rules._board.fullBoard());
                    if ( bitBoardSet(fromSq) & pinned )
                    {
                        dblPushes = rules.getNotPinnedTargets(dblPushes, kingSq, fromSq, Piece::Pawn);
                    }
                    uint64_t evasions = dblPushes & ~rules._board.bbThem() & AttackerAndEvasionPath.second;
                    moves = addTargetsAsMove(evasions, fromSq, moves, [](int){ return MoveType::DOUBLE_PUSH; }, [](int, int){ return true; }, false);
                }
                uint64_t epAttack = static_cast<bool>(rules._board.sideToMove) ? BlackPawnMap::getEpAttackTarget(fromSq, rules._board.enPassant) : WhitePawnMap::getEpAttackTarget(fromSq, rules._board.enPassant);
                if ( bitBoardSet(fromSq) & pinned )
                {
                    epAttack = rules.getNotPinnedTargets(epAttack, kingSq, fromSq, Piece::Pawn);
                }
                uint64_t epAttackSq = static_cast<bool>(rules._board.sideToMove) ? (AttackerAndEvasionPath.first >> 8) : (AttackerAndEvasionPath.first << 8); 
                uint64_t evasionsEp = epAttack & ~rules._board.bbThem() & (AttackerAndEvasionPath.second | epAttackSq);
                moves = addTargetsAsMove(evasionsEp, fromSq, moves, [](int){ return MoveType::EP_CAPTURE; }, [](int, int){ return true; }, false);
            }

            if constexpr ( GenTraits<G>::Captures )
            {
                uint64_t epAttack = static_cast<bool>(rules._board.sideToMove) ? BlackPawnMap::getEpAttackTarget(fromSq, rules._board.enPassant) : WhitePawnMap::getEpAttackTarget(fromSq, rules._board.enPassant);
                if ( bitBoardSet(fromSq) & pinned )
                {
                    epAttack = rules.getNotPinnedTargets(epAttack, kingSq, fromSq, Piece::Pawn);
                }
                // Check if Pawn making enPassant move not reveal the King (is not pinned without the strciked opposite pawn?)
                else if (epAttack)
                {
                    uint64_t striked = static_cast<bool>(rules._board.sideToMove) ? (epAttack << 8) : (epAttack >> 8);
                    uint64_t pinner = Rook::getMoves(kingSq, rules._board.bbUs()^bitBoardSet(fromSq)^epAttack, rules._board.bbThem()^striked) & (rules._board.bbThem(Piece::Rook) | rules._board.bbThem(Piece::Queen));
                    if ( pinner )
                    {
                        epAttack = 0;
                    }
                }
                moves = addTargetsAsMove(epAttack, fromSq, moves, [](int){ return MoveType::EP_CAPTURE; }, [](int, int){ return true; }, false);
            }

            return moves;
        }
    );
}
