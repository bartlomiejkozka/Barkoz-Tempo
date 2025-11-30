// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Main search engine
/*************************************************/

#include "Search.h"
#include "Evaluation.h"
#include "MoveGeneration/ChessRules.hpp"
#include "MoveGeneration/MoveGenerator.h"


[[nodiscard]] int negaMax(int depth, ChessRules &rules, int alpha, int beta, Evaluation &eval)
{
    if (depth == 0 /*|| rules._board.is_game_over()*/)
        return eval.evaluate();

    std::array<Move, 256> moves;
    int moveCount = MoveGen::generateLegalMoves(rules, moves.data());

    int maxScore = -1000000; // oo

    for (int i = 0; i < moveCount; ++i)
    {
        rules._board.makeMove(moves[i]);

        int score = -negaMax(depth - 1, rules, -beta, -alpha, eval);

        rules._board.unmakeMove();

        if (score > maxScore)
            maxScore = score;
        if (score > alpha)
            alpha = score;
        if (alpha >= beta)
            break; // beta cutoff
    }

    return maxScore;
}
