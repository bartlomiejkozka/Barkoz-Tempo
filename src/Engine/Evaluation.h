// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Position Evaluation function
/*************************************************/

#ifndef EVALUATION_H
#define EVALUATION_H

#include "MoveGeneration/ChessRules.hpp"
#include "Board.hpp"


struct Evaluation
{
    // --------------------
    // Weights
    // --------------------

    static constexpr int KingWt    = 20000;
    static constexpr int QueenWt   = 900;
    static constexpr int RookWt    = 500;
    static constexpr int BishopWt  = 300;
    static constexpr int KnightWt  = 300;
    static constexpr int PawnWt    = 100;

    static constexpr int MobilityWt = 10;

    // --------------------
    // Initialization           have to be called just after Board::init()
    // --------------------

    static void init(ChessRules &rules);

    // --------------------
    // Evaluator
    // --------------------
    
    [[nodiscard]] static int evaluate(ChessRules &rules);

    // --------------------
    // Helpers
    // --------------------

    [[nodiscard]] static int getPieceValue(PieceDescriptor piece);
};

#endif
