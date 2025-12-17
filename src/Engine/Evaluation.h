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

    // static constexpr int MobilityWt = 1;


    // King=0, Pawn=0, Knight=4, Bishop=3, Rook=2, Queen=1 -> match piece generation order as in Movegeneration genereate function
    static constexpr std::array<int, 6> MobilityWeights = { 0, 4, 0, 3, 2, 1 };

    // --------------------
    // Initialization           have to be called just after Board::init()
    // --------------------

    static void init();

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
