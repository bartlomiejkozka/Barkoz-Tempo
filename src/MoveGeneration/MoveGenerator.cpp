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
        // rules._perft_stats.checks++;

        if ( !rules.isDoubleCheck() )
        {
            moves = generate<Gen::Evasions>(rules, getKingMoves<Gen::All>(rules, moves));
        }
        // Double Check -> only King evasion
        else
        {
            // rules._perft_stats.double_checks++;
            moves = getKingMoves<Gen::All>(rules, moves);
        }
    }
    
    if ( moves == startMove ) rules._perft_stats.check_mates++;

    return static_cast<int>(moves - startMove);
}
