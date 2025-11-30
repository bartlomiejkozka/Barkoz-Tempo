// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Position Evaluation function
/*************************************************/

#ifndef EVALUATION_H
#define EVALUATION_H

#include "MoveGeneration/ChessRules.hpp"

class Evaluation
{
public:
    // --------------------
    // Weights
    // --------------------

    static constexpr int KingWt    = 200;
    static constexpr int QueenWt   = 9;
    static constexpr int RookWt    = 5;
    static constexpr int BishopWt  = 3;
    static constexpr int KnightWt  = 3;
    static constexpr int PawnWt    = 1;

    // --------------------
    // Initializer
    // --------------------

    explicit Evaluation(ChessRules &rules)
        : _rules{rules} {}

    // --------------------
    // Evaluator
    // --------------------
    
    [[nodiscard]] int evaluate() const;

    [[nodiscard]] int Pdiff(Piece piece) const;


    ChessRules &_rules;
};

#endif
