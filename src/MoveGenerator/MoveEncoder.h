// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Class for moves generation - used by ChessRules, which is considered as Higher Level Interface
/*************************************************/

#include "Move.hpp"
#include "ChessRules.hpp"
#include "Board.hpp"

#include <utility>

struct MoveEncoder
{
    [[nodiscard]] static const MoveType encodeCastling(Board _board, int targetSq) const
    {
        if ( bitBoardSet(targetSq) & KDest[static_cast<size_t>(_board.sideToMove)] )
        {
            return MoveType::KING_CASTLE;
        }
        else if ( bitBoardSet(targetSq) & QDest[static_cast<size_t>(_board.sideToMove)] )
        {
            return MoveType::QUEEN_CASTLE;
        }
        else
        {
        }

        std::unreachable();
        return MoveType::QUIET; //  this is not valid response !!!
    }
}
