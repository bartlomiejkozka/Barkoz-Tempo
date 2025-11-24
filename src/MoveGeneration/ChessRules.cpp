// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Class for cheking moves legality/pseudo legality
/*************************************************/

#include "BitOperation.hpp"
#include "ChessRules.hpp"
#include "KingPattern.hpp"
#include "KnightPattern.hpp"
#include "WhitePawnMap.hpp"
#include "BlackPawnMap.hpp"
#include "BishopMap.h"
#include "RookMap.h"
#include "QueenMap.h"
#include "Move.hpp"
#include "Board.hpp"
#include "MoveUtils.hpp"

#include <array>
#include <utility>


[[nodiscard]] uint64_t ChessRules::getCastlingMoves() const
{
    const size_t s = static_cast<size_t>(_board.sideToMove);
    
    const bool canK = (_board.castlingRights & KRight[s]) != 0;
    const bool canQ = (_board.castlingRights & QRight[s]) != 0;

    uint64_t moves = 0;
    // king-side
    if ( canK && !(KBlockers[s] & _board.fullBoard())
        && isPathSafe(KBlockers[s], static_cast<pColor>(s)) )
    {
        moves |= KDest[s];
    }
    // queen-side
    if ( canQ && !(QBlockers[s] && _board.fullBoard())
        && isPathSafe(QBlockers[s], static_cast<pColor>(s)) )
    {
        moves |= QDest[s];
    }

    return moves;
}
     

[[nodiscard]] uint64_t ChessRules::attacksTo(const int sq, const pColor attackedPColor) const
{
    const size_t attPColor = static_cast<size_t>(attackedPColor);

    const uint64_t pawns = static_cast<bool>(attackedPColor) ? 
        WhitePawnMap::attacksTo[sq] & _board.bbThem(Piece::Pawn)
        : BlackPawnMap::attacksTo[sq] & _board.bbThem(Piece::Pawn);

    const uint64_t bishop = Bishop::getMoves(sq, _board.bbUs(),  _board.bbThem()) & _board.bbThem(Piece::Bishop);
    const uint64_t rook = Rook::getMoves(sq, _board.bbUs(),  _board.bbThem()) & _board.bbThem(Piece::Rook);
    const uint64_t queen = Queen::getMoves(sq, _board.bbUs(),  _board.bbThem()) & _board.bbThem(Piece::Queen);

    return pawns | bishop | rook | queen
        | (KingPattern::attacksTo[sq] & _board.bbThem(Piece::King))
        | (KnightPattern::attacksTo[sq] & _board.bbThem(Piece::Knight));
}

[[nodiscard]] const bool ChessRules::isAttackedTo(const int sq, const pColor attackedPColor) const
{
    const uint64_t queen = Queen::getMoves(sq, _board.bbUs(),  _board.bbThem()) & _board.bbThem(Piece::Queen);
    if (queen) return true;

    const uint64_t bishop = Bishop::getMoves(sq, _board.bbUs(),  _board.bbThem()) & _board.bbThem(Piece::Bishop);
    if (bishop) return true;

    const uint64_t rook = Rook::getMoves(sq, _board.bbUs(),  _board.bbThem()) & _board.bbThem(Piece::Rook);
    if (rook) return true;

    if (KnightPattern::attacksTo[sq] & _board.bbThem(Piece::Knight)) return true;    

    if (KingPattern::attacksTo[sq] & _board.bbThem(Piece::King)) return true;

    const uint64_t pawns = static_cast<bool>(attackedPColor) ? 
        WhitePawnMap::attacksTo[sq] & _board.bbThem(Piece::Pawn)
        : BlackPawnMap::attacksTo[sq] & _board.bbThem(Piece::Pawn);
    if (pawns) return true;
    
    return false;
}

[[nodiscard]] const bool ChessRules:: isPathSafe(uint64_t pathSq, const pColor movePColor) const
{
    while (pathSq)
    {
        int sq = pop_1st(pathSq);
        if (isAttackedTo(sq, movePColor))
        {
            return false;
        }
    }
    return true;
}

[[nodiscard]] uint64_t ChessRules::getNotPinnedTargets(uint64_t targets, int kingSq, int fromSq)
{
    uint64_t legalTargets = 0;
    while ( targets )
    {
        uint64_t targetSq = minBitSet << pop_1st(targets);
        uint64_t pinner = xrayAttacks<Queen::getMoves>(kingSq, _board.bbUs()^bitBoardSet(fromSq)^targetSq, _board.bbThem(), targetSq);

        uint64_t legalSquares = 0;
        if (pinner) legalSquares = MoveUtils::inBetween[kingSq][std::countr_zero(pinner)];
        
        if ( (targetSq & legalSquares)  || (targetSq == pinner) )
        {
            legalTargets |= targetSq;
        }
    }
    
    return legalTargets;
}

[[nodiscard]] uint64_t ChessRules::getAllPins(int sq) const
{
    return getPins<Bishop::getMoves, Sliders::Bishop>(sq)
        | getPins<Rook::getMoves, Sliders::Rook>(sq);
}

[[nodiscard]] std::pair<uint64_t, uint64_t> ChessRules::getEvasions() const
{
    std::pair<uint64_t, uint64_t> res = std::make_pair(attacksTo(std::countr_zero(_board.bbUs(Piece::King)), _board.sideToMove), 0);  // only one attacker while signle check, in double check wwe should consider only King evasion

    uint64_t sliderAttackers = res.first & _board.bbThemSliders();
    int kingSq = std::countr_zero(_board.bbUs(Piece::King));
    while (sliderAttackers)     // for the check King evasion -> when slider only one Path to cover the King to evate check
    {
        int attSq = pop_1st(sliderAttackers);
        res.second |= MoveUtils::inBetween[attSq][kingSq];
    }

    return res;
}
