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

void Board::makeMove(const Move &m, uint64_t &newPoshHash)
{
    const uint64_t originSq = m.OriginSq();
    const uint64_t targetSq = m.TargetSq();
    const size_t bb = getBitboard(originSq);

    updateOriginBirboard(originSq, targetSq, bb, newPoshHash);

    if ( m.isQuiet() )
    {
        // already handled above
    }
    else if ( m.isCapture() )
    {
        newPoshHash ^= bitboards[bb + calcOpp()];

        if ( m.isEpCapture() )
        {
            bitboards[bb + calcOpp()] ^= (targetSq >> 8);
        }
        else
        {
            bitboards[bb + calcOpp()] ^= targetSq;
        }

        newPoshHash ^= bitboards[bb + calcOpp()];
    }
    else if ( m.isPromotion() )
    {
        newPoshHash ^= bitboards[bb];

        // Promotion Capture
        if ( m.isAnyCapture() )
        {
            newPoshHash ^= bitboards[bb + calcOpp()];
            bitboards[bb + calcOpp()] ^= targetSq;
            newPoshHash ^= bitboards[bb + calcOpp()];
        }

        // For now asume we do promotion only to Queen
        bitboards[bb] ^= targetSq;

        size_t idx = PieceDescriptor::bQueen - (sideToMove ? 0 : 1);
        newPoshHash ^= bitboards[idx];
        bitboards[idx] ^= targetSq;
        newPoshHash ^= bitboards[idx];

        newPoshHash ^= bitboards[bb];
    }
    else if ( m.isQueenCastle )
    {
        size_t idx = PieceDescriptor::bRook - (sideToMove ? 0 : 1);
        newPoshHash ^= bitboards[idx];
        setBbUs(Piece::Rook, targetSq >> 3);
        newPoshHash ^= bitboards[idx] ^ ;
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

void Board::updateOriginBirboard(const uint64_t originSq, const uint64_t targetSq, const size_t bbN, uint64_t &poshHash);
{
    const uint64_t fromToBB = originSq ^ targetSq;
    
    posHash ^= PieceMap::pieceMap[bbN - 2][originSq];
    posHash ^= PieceMap::pieceMap[bbN - 2][targetSq];

    bitboards[bbN] ^= fromToBB;

    posHash ^= PieceMap::pieceMap[bbN - 2][originSq];
    posHash ^= PieceMap::pieceMap[bbN - 2][targetSq];
}
