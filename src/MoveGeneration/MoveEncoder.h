// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Class for moves generation - used by ChessRules, which is considered as Higher Level Interface
/*************************************************/

#ifndef MOVE_ENCODER_H
#define MOVE_ENCODER_H

#include "Move.hpp"
#include "ChessRules.hpp"
#include "Board.hpp"
#include "BitOperation.hpp"

#include <utility>

struct MoveEncoder
{
    [[nodiscard]] static const MoveType encodeCastling(Board _board, int targetSq)
    {
        if ( bitBoardSet(targetSq) & ChessRules::KDest[static_cast<size_t>(_board.sideToMove)] )
        {
            return MoveType::KING_CASTLE;
        }
        else if ( bitBoardSet(targetSq) & ChessRules::QDest[static_cast<size_t>(_board.sideToMove)] )
        {
            return MoveType::QUEEN_CASTLE;
        }
        else
        {
        }

        // std::unreachable();
        return MoveType::QUIET; //  this is not valid response !!!
    }
};

#endif
