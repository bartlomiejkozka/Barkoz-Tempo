// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Main search engine
/*************************************************/

#include "Search.h"
#include "Evaluation.h"
#include "MoveGeneration/ChessRules.hpp"
#include "MoveGeneration/MoveGenerator.h"
#include "MoveParser.h"

#include <limits>

static constexpr int INF = std::numeric_limits<int>::max();

// TODO: make current Pos. score calcualtion incremental, not calculatingevery time !!!!!!!!!!!!!!!!!!!!!!!!!!

[[nodiscard]] int negaMax(int depth, ChessRules &rules, int alpha, int beta)
{
    if (depth == 0 /*|| rules._board.is_game_over()*/)
        return Evaluation::evaluate(rules);

    std::array<Move, 256> moves;
    int moveCount = MoveGen::generateLegalMoves(rules, moves.data());

    int maxScore = -1000000;

    for (int i = 0; i < moveCount; ++i)
    {
        rules._board.makeMove(moves[i]);

        int score = -negaMax(depth - 1, rules, -beta, -alpha);

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

[[nodiscard]] int minMax(ChessRules &rules, int depth, int alpha, int beta, bool isMaxTurn)
{
    if (depth == 0)
    {
        return Evaluation::evaluate(rules);
    }

    std::array<Move, 256> moves;
    int moveCount = MoveGen::generateLegalMoves(rules, moves.data());

    if (isMaxTurn)
    {        
        int maxScore = std::numeric_limits<int>::min();

        for (int i = 0; i < moveCount; ++i)
        {
            rules._board.makeMove(moves[i]);

            int score = minMax(rules, depth - 1, alpha, beta, false);

            rules._board.unmakeMove();

            if (score > maxScore)
            {
                maxScore = score;
            }
            if (score > alpha)
            {
                alpha = score;
            }
            // pruning
            if (alpha >= beta)
            {
                break;
            }
        }
        return maxScore;
    }
    else
    {
        int minScore = std::numeric_limits<int>::max();

        for (int i = 0; i < moveCount; ++i)
        {
            rules._board.makeMove(moves[i]);

            int score = minMax(rules, depth - 1, alpha, beta, true);

            rules._board.unmakeMove();

            if (score < minScore)
            {
                minScore = score;
            }
            if (score < beta)
            {
                beta = score;
            }
            // pruning
            if (beta <= alpha)
            {
                break;
            }
        }
        return minScore;
    }
}

void SearchDivideMinimax(int depth, ChessRules &rules) 
{
    std::array<Move, 256> moves;
    int moveCount = MoveGen::generateLegalMoves(rules, moves.data());

    std::cout << "--- Start Search Divide (Depth " << depth << ") ---\n";
    std::cout << "Move | Score \n";
    std::cout << "-----|-------\n";

    // 1. Sprawdzamy, kim jesteśmy w korzeniu
    bool amIWhite = (rules._board.sideToMove == pColor::White);
    
    // 2. Ustawiamy startowy "najlepszy wynik"
    // Jeśli jestem Biały (MAX), szukam liczby większej od -INF.
    // Jeśli jestem Czarny (MIN), szukam liczby mniejszej od +INF.
    int bestScore = amIWhite ? -INF : INF;
    Move bestMove;
    bool foundAny = false;

    for (int i = 0; i < moveCount; ++i) 
    {
        rules._board.makeMove(moves[i]);

        // 3. Wywołanie Minimax
        // - depth - 1: schodzimy głębiej
        // - alpha, beta: przekazujemy pełne okno (-INF, INF)
        // - !amIWhite: 
        //     Jeśli ja jestem Biały (true), to moje dziecko jest Czarny (false/Minimizing).
        //     Jeśli ja jestem Czarny (false), to moje dziecko jest Biały (true/Maximizing).
        
        // WAŻNE: Tu NIE MA minusa przed funkcją!
        int score = minMax(rules, depth - 1, -INF, INF, !amIWhite);

        rules._board.unmakeMove();

        // Wyświetlanie
        std::string moveStr = SimpleParser::moveToString(moves[i].OriginSq(), moves[i].TargetSq());
        
        std::string scoreStr;
        if (score > 29000) scoreStr = "+Mate";
        else if (score < -29000) scoreStr = "-Mate";
        else scoreStr = std::to_string(score);

        std::cout << std::left << std::setw(5) << moveStr << "| " << scoreStr << "\n";

        // 4. Aktualizacja najlepszego ruchu zależnie od koloru
        if (amIWhite) {
            // Białe chcą jak najwięcej
            if (score > bestScore) {
                bestScore = score;
                bestMove = moves[i];
                foundAny = true;
            }
        } else {
            // Czarne chcą jak najmniej
            if (score < bestScore) {
                bestScore = score;
                bestMove = moves[i];
                foundAny = true;
            }
        }
    }

    std::cout << "-----------------\n";
    if (foundAny) {
        std::cout << "Best Move: " << SimpleParser::moveToString(bestMove.OriginSq(), bestMove.TargetSq()) 
                  << " (Score: " << bestScore << ")\n";
    }
}
