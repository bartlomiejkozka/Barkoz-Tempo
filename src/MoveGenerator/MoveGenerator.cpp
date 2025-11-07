// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Class for moves generation - used by ChessRules, which is considered as Higher Level Interface
/*************************************************/

#include "MoveGenerator.h"
#include "Board.hpp"


// -------------------------
// Piece Types Helpers
// -------------------------

[[nodiscard]] Move* getKingMove(Board &_board, Move *moves);
{
    const int kingSq = count_1s(_board.bbUs(Piece::King));

    uint64_t movesR = KingPattern::getMoves(static_cast<size_t>(kingSq), _board.bbUs());
    while (movesR)
    {
        int sq = pop_1st(movesR);
        if ( !isAttackedTo(sq) )
        {
            *moves++ = Move(kingSq, sq, encodeKingKnightMoveTypePure(sq));
        }
    }

    // add castling moves
    uint64_t castlings = getCastlingMoves();
    while (castlings)
    {
        int sq = pop_1st(castlings);
        *moves++ = Move(kingSq, sq, encodeCastling(sq));
    }

    return moves;
}   

[[nodiscard]] Move* getKnightMove(Board &_board, Move *moves)
{
    uint64_t moves = KnightPattern::getMoves(static_cast<size_t>(kingSq), _board.bbUs());

}

