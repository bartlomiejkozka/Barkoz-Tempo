// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Class for cheking moves legality/pseudo legality
/*************************************************/

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
    if ( canK && (KBlockers[s] && _board.fullBoard()) )
    {
        moves |= KDest[s];
    }
    // queen-side
    if ( canQ && (QBlockers[s] && _board.fullBoard()) )
    {
        moves |= QDest[s];
    }

    return moves;
}


[[nodiscard]] const uint64_t ChessRules::attacksTo(const int sq, const pColor attackedPColor) const
{
    const uint64_t pawns = attackedPColor ? BlackPawnMap::attacksTo[sq] & _board.bitboards[PieceDescriptor::wPawn + attackedPColor]
        : WhitePawnMap::attacksTo[sq] & _board.bitboards[PieceDescriptor::wPawn + attackedPColor];

    const uint64_t bishop = BishopMap::getMoves(sq, _board.bitboards[PieceDescriptor::bBishop - attackedPColor],  _board.bitboards[PieceDescriptor::bBishop + attackedPColor]);
    const uint64_t rook = RookMap::getMoves(sq, _board.bitboards[PieceDescriptor::bRook - attackedPColor],  _board.bitboards[PieceDescriptor::bRook + attackedPColor]);
    const uint64_t queen = QueenMap::getMoves(sq, _board.bitboards[PieceDescriptor::bQueen - attackedPColor],  _board.bitboards[PieceDescriptor::bQueen + attackedPColor]);

    return pawns | bishop | rook | queen
        | (KingPattern::attacksTo[sq] & _board.bitboards[PieceDescriptor::wKing + attackedPColor])
        | (KnightPattern::attacksTo[sq] & _board.bitboards[PieceDescriptor::wKing + attackedPColor]);
}
 