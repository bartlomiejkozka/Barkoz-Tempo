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
#include "TranspositionTable.h"
#include "Board.hpp"
#include "BitOperation.hpp"
#include "MoveParser.h"

#include <limits>


static constexpr int INF = std::numeric_limits<int>::max();


void Search::orderMoves(std::array<Move, 256> &moves, int count, Move hashMove, ChessRules& rules) 
{
    struct ScoredMove
    {
        Move move;
        int score;
        
        bool operator>(const ScoredMove& other) const 
        {
            return score > other.score;
        }
    };

    std::array<ScoredMove, 256> scoredMoves;

    for (int i = 0; i < count; ++i) 
    {
        int score = 0;
        Move currentMove = moves[i];

        if ((currentMove.TargetSq() == hashMove.TargetSq()) && (currentMove.OriginSq() == hashMove.OriginSq())) 
        {
            score = 2000000;
        }
        else if (currentMove.isAnyCapture()) 
        {
            PieceDescriptor aggressor = static_cast<PieceDescriptor>(
                rules._board.getBitboard(bitBoardSet(currentMove.OriginSq()))
            );
            
            PieceDescriptor victim;
            if (currentMove.isEpCapture()) 
            {
                victim = PieceDescriptor::wPawn; 
            } 
            else 
            {
                victim = static_cast<PieceDescriptor>(
                    rules._board.getBitboard(bitBoardSet(currentMove.TargetSq()))
                );
            }

            score = (Evaluation::getPieceValue(victim) * 10) - Evaluation::getPieceValue(aggressor) + 10000; 
        }
        
        scoredMoves[i] = {currentMove, score};
    }

    std::sort(scoredMoves.begin(), scoredMoves.begin() + count, 
        [](const ScoredMove& a, const ScoredMove& b) 
        {
            return a.score > b.score;
        }
    );

    for (int i = 0; i < count; ++i) 
    {
        moves[i] = scoredMoves[i].move;
    }
}

[[nodiscard]] int Search::quiescence(ChessRules &rules, int alpha, int beta, bool isMaxTurn)
{
    int stand_pat = Evaluation::evaluate(rules);

    if (isMaxTurn)
    {
        if (stand_pat >= beta) return beta;
        if (stand_pat > alpha) alpha = stand_pat;
    }
    else
    {
        if (stand_pat <= alpha) return alpha;
        if (stand_pat < beta) beta = stand_pat;
    }

    // for now only fighting captures check
    std::array<Move, 256> captures;
    Move *endPtr = MoveGen::generate<Gen::Captures>(rules, captures.data());
    auto count = static_cast<int>(endPtr - captures.data());

    orderMoves(captures, count, Move{0}, rules);

    for (int i = 0; i < count; ++i)
    {
        rules._board.makeMove(captures[i]);
        
        int score = quiescence(rules, alpha, beta, !isMaxTurn);
        
        rules._board.unmakeMove();

        if (isMaxTurn)
        {
            if (score >= beta) return beta;
            if (score > alpha) alpha = score;
        }
        else
        {
            if (score <= alpha) return alpha;
            if (score < beta) beta = score;
        }
    }

    return isMaxTurn ? alpha : beta;
}

[[nodiscard]] int Search::minMax(ChessRules &rules, int depth, int alpha, int beta, bool isMaxTurn)
{
    nodes++;

    if ((nodes & 2047) == 0) {
        checkTime();
    }

    if ( rules.isRepetition() ) return 0;   // when 2fold repetition

    using TTEntry = TranspositionTable::Entry;
    
    TTEntry ttEntry = _TT.probe(rules._board.zobristKey);
    
    if (ttEntry.isValid() && ttEntry.depth >= depth) 
    {
        if (ttEntry.type == TTEntry::Type::EXACT)                                return ttEntry.score;
        if (ttEntry.type == TTEntry::Type::LOWERBOUND && ttEntry.score >= beta)  return ttEntry.score;
        if (ttEntry.type == TTEntry::Type::UPPERBOUND && ttEntry.score <= alpha) return ttEntry.score;
    }

    Move hashMove = ttEntry.isValid() ? ttEntry.move : Move{0};

    if (depth == 0)
    {
        return quiescence(rules, alpha, beta, isMaxTurn);
    }

    std::array<Move, 256> moves;
    int moveCount = MoveGen::generateLegalMoves(rules, moves.data());

    if (moveCount == 0)
    {
        // Mate
        if (rules.isCheck()) 
        {
            const int MATE_SCORE = 100000; 

            if (isMaxTurn) 
                return -MATE_SCORE - depth;
            else 
                return MATE_SCORE + depth;
        }
        // Pat
        else 
        {
            return 0;
        }
    }

    // order Moves
    orderMoves(moves, moveCount, hashMove, rules);

    if (isMaxTurn)
    {        
        int maxScore = std::numeric_limits<int>::min();
        Move bestMove = Move{0};
        int originAlpha = alpha;

        for (int i = 0; i < moveCount; ++i)
        {
            rules._board.makeMove(moves[i]);

            int score = minMax(rules, depth - 1, alpha, beta, false);

            rules._board.unmakeMove();

            if (stopRequest) return 0;

            if (score > maxScore)
            {
                maxScore = score;
                bestMove = moves[i];
            }
            if (score > alpha)
            {
                alpha = score;
            }
            // pruning
            if (score >= beta)
            {
                _TT.save(rules._board.zobristKey, depth, score, TTEntry::Type::LOWERBOUND, moves[i]);
                return score;
            }
        }

        TTEntry::Type type = (maxScore <= originAlpha) ? TTEntry::Type::UPPERBOUND : TTEntry::Type::EXACT;
        _TT.save(rules._board.zobristKey, depth, maxScore, type, bestMove);
        return maxScore;
    }
    else
    {
        int minScore = std::numeric_limits<int>::max();
        Move bestMove = Move{0};
        int originBeta = beta;

        for (int i = 0; i < moveCount; ++i)
        {
            rules._board.makeMove(moves[i]);

            int score = minMax(rules, depth - 1, alpha, beta, true);

            rules._board.unmakeMove();

            if (stopRequest) return 0;

            if (score < minScore)
            {
                minScore = score;
                bestMove = moves[i];
            }
            if (score < beta)
            {
                beta = score;
            }
            // pruning
            if (score <= alpha)
            {
                _TT.save(rules._board.zobristKey, depth, score, TTEntry::Type::UPPERBOUND, moves[i]);
                return score;
            }
        }

        TTEntry::Type type = (minScore >= originBeta) ? TTEntry::Type::LOWERBOUND : TTEntry::Type::EXACT;
        _TT.save(rules._board.zobristKey, depth, minScore, type, bestMove);
        return minScore;
    }
}

Move Search::searchPosition(ChessRules &rules, int maxDepth, int timeInMillis) 
{
    Move bestRootMove = Move{0};

    stopRequest = false;
    nodes = 0;
    startTime = std::chrono::steady_clock::now();
    allocatedTime = timeInMillis;

    bool isMaxTurn = (rules._board.sideToMove == pColor::White);

    for (int depth = 1; depth <= maxDepth; ++depth) 
    {
        int score = minMax(rules, depth, -INF, INF, isMaxTurn);
        
        if (stopRequest)
        {
            break;
        }

        using Entry = TranspositionTable::Entry;
        Entry ttEntry = _TT.probe(rules._board.zobristKey);

        if (ttEntry.isValid()) 
        {
            std::array<Move, 256> legalMoves;
            int count = MoveGen::generateLegalMoves(rules, legalMoves.data());
            
            bool isLegal = false;
            for(int i=0; i<count; ++i) 
            {
                if (legalMoves[i].OriginSq() == ttEntry.move.OriginSq() &&
                    legalMoves[i].TargetSq() == ttEntry.move.TargetSq()) 
                {
                    bestRootMove = legalMoves[i]; 
                    isLegal = true;
                    break;
                }
            }

            if (!isLegal && bestRootMove.getPackedMove() == 0 && count > 0) 
            {
                bestRootMove = legalMoves[0];
            }
        }

        std::string moveStr = SimpleParser::moveToString(bestRootMove.OriginSq(), bestRootMove.TargetSq());
        std::cout << " info depth " << depth;
        std::cout << " score cp " << ttEntry.score;
        std::cout << " nodes " << nodes;
        std::cout << " pv " << moveStr << std::endl;
    }

    std::string moveStr = SimpleParser::moveToString(bestRootMove.OriginSq(), bestRootMove.TargetSq());
    if (char promChar = SimpleParser::promotionTypeToChar(bestRootMove.getType()); 
        promChar != '\0')
    { 
        moveStr += promChar;
    }
    std::cout << "bestmove " << moveStr << std::endl;

    return bestRootMove;
}

void Search::SearchDivideMinimax(int depth, ChessRules &rules) 
{
    std::array<Move, 256> moves;
    int moveCount = MoveGen::generateLegalMoves(rules, moves.data());

    std::cout << "--- Start Search Divide (Depth " << depth << ") ---\n";
    std::cout << "Move | Score \n";
    std::cout << "-----|-------\n";

    bool amIWhite = (rules._board.sideToMove == pColor::White);
    
    int bestScore = amIWhite ? -INF : INF;
    Move bestMove;
    bool foundAny = false;

    for (int i = 0; i < moveCount; ++i) 
    {
        rules._board.makeMove(moves[i]);

       int score = minMax(rules, depth - 1, -INF, INF, !amIWhite);

        rules._board.unmakeMove();

        std::string moveStr = SimpleParser::moveToString(moves[i].OriginSq(), moves[i].TargetSq());
        
        std::string scoreStr;
        if (score > 29000) scoreStr = "+Mate";
        else if (score < -29000) scoreStr = "-Mate";
        else scoreStr = std::to_string(score);

        std::cout << std::left << std::setw(5) << moveStr << "| " << scoreStr << "\n";

        if (amIWhite) {
            if (score > bestScore) {
                bestScore = score;
                bestMove = moves[i];
                foundAny = true;
            }
        } else {
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

void Search::checkTime() 
{
    if (stopRequest) return;

    if (allocatedTime > 0)
    {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
        
        if (elapsed >= allocatedTime) 
        {
            stopRequest = true;
        }
    }
}
