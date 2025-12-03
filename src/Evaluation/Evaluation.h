// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Position Evaluation function
/*************************************************/

#ifndef EVALUATION_H
#define EVALUATION_H

#include "MoveGeneration/ChessRules.hpp"

struct Evaluation
{
    // --------------------
    // Weights
    // --------------------

    static constexpr int KingWt    = 200;
    static constexpr int QueenWt   = 9;
    static constexpr int RookWt    = 5;
    static constexpr int BishopWt  = 3;
    static constexpr int KnightWt  = 3;
    static constexpr int PawnWt    = 1;

    static constexpr int MobilityWt = 1;

    // --------------------
    // Evaluator
    // --------------------
    
    [[nodiscard]] static int evaluate(ChessRules &rules);
};

#endif
