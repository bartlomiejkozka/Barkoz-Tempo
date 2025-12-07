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

#include <atomic>
#include <chrono>

class Search
{
private:
    TranspositionTable &_TT;
    std::chrono::steady_clock::time_point startTime;
    int allocatedTime;
    uint64_t nodes;         // nodes counter

    void orderMoves(std::array<Move, 256> &moves, int count, Move hashMove, ChessRules& rules);

    [[nodiscard]] int minMax(ChessRules &rules, int depth, int alpha, int beta, bool isMaxTurn);

    void checkTime();

public:
    std::atomic<bool> stopRequest = false;

    // ---------------------
    // Initizaliztion
    // ---------------------

    explicit Search(TranspositionTable &TT)
        : _TT{TT} {}

    // ---------------------
    // Methods
    // ---------------------

    Move searchPosition(ChessRules &rules, int maxDepth, int timeInMillis);

    void clear() { _TT.clear(); }

    void SearchDivideMinimax(int depth, ChessRules &rules);
};

#endif
