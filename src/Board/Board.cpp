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
    const size_t bbN = getBitboard(originSq);
    updateOriginBirboard(originSq, targetSq, bbN);

    if ( m.isCapture() )
    {
        if ( m.isEpCapture() )
        {
            bitboards[bbN + calcOpp()] ^= (targetSq >> 8);
        }

        bitboards[bbN + calcOpp()] ^= targetSq;
    }
    else if (m.isKingCastle())
    {

    }
    else if (m.isQueenCastle())
    {

    }
    
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
