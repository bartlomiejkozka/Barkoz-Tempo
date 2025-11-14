// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Game logic holder
/*************************************************/

#include "Board.hpp"
#include "MoveUtils.hpp"

#include <utility>

// ---------------------------------
// Move make
// ---------------------------------

void Board::makeMove(Move &m)
{
    const uint64_t originSq = m.OriginSq();
    const uint64_t targetSq = m.TargetSq();
    const size_t bb = getBitboard(originSq);

    updateOriginBirboard(originSq, targetSq, bb);

    if ( m.isQuiet() )
    {
        // already handled above
    }
    else if ( m.isCapture() )
    {
        if ( m.isEpCapture() )
        {
            bitboards[bb + calcOpp()] ^= (targetSq >> 8);
        }
        else
        {
            bitboards[bb + calcOpp()] ^= targetSq;
        }
    }
    else if ( m.isPromotion() )
    {
        // Promotion Capture
        if ( m.isAnyCapture() )
        {
            bitboards[bb + calcOpp()] ^= targetSq;
        }

        // For now asume we do promotion only to Queen
        bitboards[bb] ^= targetSq;
        bitboards[PieceDescriptor::bQueen - (sideToMove ? 0 : 1)] ^= targetSq;
    }
    else if ( m.isQueenCastle )
    {
        setBbUs(Piece::Rook, targetSq >> 3);
    }
    else if ( m.isKingCastle() )
    {
        setBbUs(Piece::Rook, targetSq << 2);
    }

    // update: en-passant, half-move clk, 
    // TODO: add zobrist hash to Undo table and add game hist table
}

void Board::unmakeMove()
{

}

// ---------------------------------
// Move make Helpers
// ---------------------------------

const size_t Board::getBitboard(const uint64_t sq)
{
    for (int i = 2; i < bitboardCount; ++i)
    {
        if ( sq & bitboards[i] ) return i;
    }

    std::unreachable();
    return 0;
}

void Board::updateOriginBirboard(const uint64_t originSq, const uint64_t targetSq, const size_t bbN);
{
    const uint64_t fromToBB = originSq ^ targetSq;
    bitboards[bbN] ^= fromToBB;
}
