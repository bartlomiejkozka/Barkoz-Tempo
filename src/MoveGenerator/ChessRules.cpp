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


[[nodiscard]] const uint64_t ChessRules::getCastlingMoves() const
{
    const size_t s = static_cast<int>(_board.sideToMove);
    
    const bool canK = (_board.castlingRights & KRight[s]) != 0;
    const bool canQ = (_board.castlingRights & QRight[s]) != 0;

    const uint64_t moves = 0;
    // king-side
    if ( canK && (KBlockers[s] & _board.fullBoard())
        && isPathSafe(KBlockers[s], static_cast<pColor>s) )
    {
        moves |= KDest[s];
    }
    // queen-side
    if ( canQ && (QBlockers[s] && _board.fullBoard())
        && isPathSafe(QBlockers[s], static_cast<pColor>s) )
    {
        moves |= QDest[s];
    }

    return moves;
}
     

[[nodiscard]] uint64_t ChessRules::attacksTo(const int sq, const pColor attackedPColor) const
{
    const uint64_t pawns = attackedPColor ? BlackPawnMap::attacksTo[sq] & _board.bitboards[PieceDescriptor::wPawn + attackedPColor]
        : WhitePawnMap::attacksTo[sq] & _board.bitboards[PieceDescriptor::wPawn + attackedPColor];

    const uint64_t bishop = BishopMap::getMoves(sq, _board.bitboards[PieceDescriptor::bBishop - attackedPColor],  _board.bitboards[PieceDescriptor::bBishop + attackedPColor]);
    const uint64_t rook = RookMap::getMoves(sq, _board.bitboards[PieceDescriptor::bRook - attackedPColor],  _board.bitboards[PieceDescriptor::bRook + attackedPColor]);
    const uint64_t queen = QueenMap::getMoves(sq, _board.bitboards[PieceDescriptor::bQueen - attackedPColor],  _board.bitboards[PieceDescriptor::bQueen + attackedPColor]);

    return pawns | bishop | rook | queen
        | (KingPattern::attacksTo[sq] & _board.bitboards[PieceDescriptor::wKing + attackedPColor])
        | (KnightPattern::attacksTo[sq] & _board.bitboards[PieceDescriptor::wKnight + attackedPColor]);
}

[[nodiscard]] const bool isAttackedTo(const int sq, const pColor movePColor) const
{
    const uint64_t queen = QueenMap::getMoves(sq, _board.bitboards[PieceDescriptor::bQueen - attackedPColor],  _board.bitboards[PieceDescriptor::bQueen + attackedPColor]);
    if (queen) return true;

    const uint64_t bishop = BishopMap::getMoves(sq, _board.bitboards[PieceDescriptor::bBishop - attackedPColor],  _board.bitboards[PieceDescriptor::bBishop + attackedPColor]);
    if (bishop) return true;

    const uint64_t rook = RookMap::getMoves(sq, _board.bitboards[PieceDescriptor::bRook - attackedPColor],  _board.bitboards[PieceDescriptor::bRook + attackedPColor]);
    if (rook) return true;

    if (KnightPattern::attacksTo[sq] & _board.bitboards[PieceDescriptor::wKnight + attackedPColor]) return true;    

    if (KingPattern::attacksTo[sq] & _board.bitboards[PieceDescriptor::wKing + attackedPColor]) return true;

    const uint64_t pawns = attackedPColor ? BlackPawnMap::attacksTo[sq] & _board.bitboards[PieceDescriptor::wPawn + attackedPColor]
        : WhitePawnMap::attacksTo[sq] & _board.bitboards[PieceDescriptor::wPawn + attackedPColor];
    if (pawns) return true;
    
    return false;
}

[[nodiscard]] const bool isPathSafe(uint64_t pathSq, const pColor movePColor) const
{
    while (pathSq) 
    {
        int sq = pop_1st(pathSq);
        if (isAttacksTo(sq, movePColor))
        {
            return false;
        }
    }
    return true;
}

// blockers: by default it is attacked pieces bitboard
// use example to get pinners: xrayAttacks() & bbThem;
template <uint64_t (*Attacks)(const int, const uint64_t, const uint64_t)>
[[nodiscard]] uint64_t ChessRules::xrayAttacks(int sq, uint64_t bbUs, uint64_t bbThem, uint64_t blockers) const
{
    uint64_t attacks = Attacks(sq, bbUs, bbThem);
    blockers &= attacks;

    // retuns the attacks behind the pin when there was a pinner
    return attacks ^ Attacks(sq, bbUs, bbThem ^ blockers);
}
