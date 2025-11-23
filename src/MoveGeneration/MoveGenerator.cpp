// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Class for moves generation - used by ChessRules, which is considered as Higher Level Interface
/*************************************************/

#include "MoveGenerator.h"
#include "Board.hpp"

[[nodiscard]] int MoveGen::generateLegalMoves(ChessRules &rules, Move *moves)
{
    Move const *startMove = moves;

    if ( !rules.isCheck() )
    {
        moves = generate<Gen::All>(rules, moves);
    }
    else
    {
        if ( !rules.isDoubleCheck() )
        {
            moves = generate<Gen::Evasions>(rules, getKingMoves<Gen::All>(rules, moves));
        }
        // Double Check -> only King evasion
        else
        {
            moves = getKingMoves<Gen::All>(rules, moves);
        }
    }
    
    return static_cast<int>(moves - startMove);
}
