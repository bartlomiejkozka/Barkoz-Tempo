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


[[nodiscard]] const uint64_t ChessRules::getCastlingMoves() const
{
    const size_t s = static_cast<size_t>(_board.sideToMove);
    
    const bool canK = (_board.castlingRights & KRight[s]) != 0;
    const bool canQ = (_board.castlingRights & QRight[s]) != 0;

    uint64_t moves = 0;
    // king-side
    if ( canK && (KBlockers[s] & _board.fullBoard())
        && isPathSafe(KBlockers[s], static_cast<pColor>(s)) )
    {
        moves |= KDest[s];
    }
    // queen-side
    if ( canQ && (QBlockers[s] && _board.fullBoard())
        && isPathSafe(QBlockers[s], static_cast<pColor>(s)) )
    {
        moves |= QDest[s];
    }

    return moves;
}
     

[[nodiscard]] uint64_t ChessRules::attacksTo(const int sq, const pColor attackedPColor) const
{
    const size_t attPColor = static_cast<size_t>(attackedPColor); 

    const uint64_t pawns = static_cast<bool>(attackedPColor) ? BlackPawnMap::attacksTo[sq] & _board.bitboards[static_cast<size_t>(PieceDescriptor::wPawn) + attPColor]
        : WhitePawnMap::attacksTo[sq] & _board.bitboards[static_cast<size_t>(PieceDescriptor::wPawn) + attPColor];

    const uint64_t bishop = Bishop::getMoves(sq, _board.bitboards[static_cast<size_t>(PieceDescriptor::bBishop) - attPColor],  _board.bitboards[static_cast<size_t>(PieceDescriptor::bBishop) + attPColor]);
    const uint64_t rook = Rook::getMoves(sq, _board.bitboards[static_cast<size_t>(PieceDescriptor::bRook) - attPColor],  _board.bitboards[static_cast<size_t>(PieceDescriptor::bRook) + attPColor]);
    const uint64_t queen = Queen::getMoves(sq, _board.bitboards[static_cast<size_t>(PieceDescriptor::bQueen) - attPColor],  _board.bitboards[static_cast<size_t>(PieceDescriptor::bQueen) + attPColor]);

    return pawns | bishop | rook | queen
        | (KingPattern::attacksTo[sq] & _board.bitboards[static_cast<size_t>(PieceDescriptor::wKing) + attPColor])
        | (KnightPattern::attacksTo[sq] & _board.bitboards[static_cast<size_t>(PieceDescriptor::wKnight) + attPColor]);
}

[[nodiscard]] const bool ChessRules::isAttackedTo(const int sq, const pColor attackedPColor) const
{
    const size_t attPColor = static_cast<size_t>(attackedPColor); 

    const uint64_t queen = Queen::getMoves(sq, _board.bitboards[static_cast<size_t>(PieceDescriptor::bQueen) - attPColor],  _board.bitboards[static_cast<size_t>(PieceDescriptor::bQueen) + attPColor]);
    if (queen) return true;

    const uint64_t bishop = Bishop::getMoves(sq, _board.bitboards[static_cast<size_t>(PieceDescriptor::bBishop) - attPColor],  _board.bitboards[static_cast<size_t>(PieceDescriptor::bBishop) + attPColor]);
    if (bishop) return true;

    const uint64_t rook = Rook::getMoves(sq, _board.bitboards[static_cast<size_t>(PieceDescriptor::bRook) - attPColor],  _board.bitboards[static_cast<size_t>(PieceDescriptor::bRook) + attPColor]);
    if (rook) return true;

    if (KnightPattern::attacksTo[sq] & _board.bitboards[static_cast<size_t>(PieceDescriptor::wKnight) + attPColor]) return true;    

    if (KingPattern::attacksTo[sq] & _board.bitboards[static_cast<size_t>(PieceDescriptor::wKing) + attPColor]) return true;

    const uint64_t pawns = static_cast<bool>(attackedPColor) ? BlackPawnMap::attacksTo[sq] & _board.bitboards[static_cast<size_t>(PieceDescriptor::wPawn) + attPColor]
        : WhitePawnMap::attacksTo[sq] & _board.bitboards[static_cast<size_t>(PieceDescriptor::wPawn) + attPColor];
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

[[nodiscard]] uint64_t ChessRules::getAllPins(int sq) const
{
    return getPins<Bishop::getMoves, Sliders::Bishop>(sq)
        | getPins<Rook::getMoves, Sliders::Rook>(sq);
}

[[nodiscard]] std::pair<uint64_t, uint64_t> ChessRules::getEvasions() const
{
    std::pair<uint64_t, uint64_t> res = std::make_pair(attacksTo(_board.bbUs(Piece::King), _board.sideToMove), 0);  // only one attacker while signle check, in double check wwe should consider only King evasion

    uint64_t sliderAttackers = res.first & _board.bbThemSliders();
    int kingSq = count_1s(_board.bbUs(Piece::King));
    while (sliderAttackers)     // for the check King evasion -> when slider only one Path to cover the King to evate check
    {
        int attSq = pop_1st(sliderAttackers);
        res.second |= MoveUtils::inBetween[attSq][kingSq];
    }

    return res;
}
