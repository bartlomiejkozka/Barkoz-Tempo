// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Main search engine
/*************************************************/

#ifndef SEARCH_H
#define SEARCH_H

#include "MoveGeneration/ChessRules.hpp"
#include "TranspositionTable.h"
#include "MoveGeneration/Move.hpp"

class Search
{
private:
    TranspositionTable &_TT;

public:
    // ---------------------
    // Initizaliztion
    // ---------------------

    explicit Search(TranspositionTable &TT)
        : _TT{TT} {}

    // ---------------------
    // Methods
    // ---------------------

    void orderMoves(std::array<Move, 256> &moves, int count, Move hashMove, ChessRules& rules);

    [[nodiscard]] int minMax(ChessRules &rules, int depth, int alpha, int beta, bool isMaxTurn);

    Move searchPosition(ChessRules &rules, int maxDepth);

    void SearchDivideMinimax(int depth, ChessRules &rules);
};

#endif
