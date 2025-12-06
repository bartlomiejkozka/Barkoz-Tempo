#include "PerftFunctions.h"
#include "MoveParser.h"
#include "MoveGeneration/Move.hpp"
#include "MoveGeneration/MoveGenerator.h"


static void countMoveStatic(Move &m, PerftStats &stats) 
{
    if (m.isAnyCapture()) stats.captures++;
    if (m.isEpCapture()) stats.enPassantCaptures++;
    if (m.isPromotion()) stats.promotions++;
    if (m.isCastle()) stats.castles++;
}

static void countMoveStaticCheck(ChessRules &rules, PerftStats &stats, Move &m)
{
    if ( rules.isCheck() )
    {
        stats.checks++;
        if ( rules.isDoubleCheck() ) stats.double_checks++;
        if ( rules.getAllPins(m.OriginSq()) )    stats.discovery_checks++;
        // stats.check_mates already handled in MoveGen::generate function.
    }
}

uint64_t Perft(int depth, ChessRules &rules, PerftStats &stats)
{
    if (depth == 0) {
        stats.nodes += 1;
        return 1;
    }


    std::array<Move, 256> move_list;
    int n_moves = MoveGen::generateLegalMoves(rules, move_list.data());

    uint64_t nodes = 0;

    for (int i = 0; i < n_moves; ++i) 
    {
        countMoveStatic(move_list[i], stats);

        rules._board.makeMove(move_list[i]);

        countMoveStaticCheck(rules, stats, move_list[i]);

        nodes += Perft(depth - 1, rules, stats);

        rules._board.unmakeMove();
    }

    return nodes;
}

uint64_t PerftDivide(int depth, ChessRules &rules, PerftStats &stats) 
{
    std::array<Move, 256> moves;
    int moveCount = MoveGen::generateLegalMoves(rules, moves.data());

    uint64_t total = 0;

    for (int i = 0; i < moveCount; ++i) 
    {
        // optionally snapshot per-branch stats
        PerftStats branchStats;
        branchStats.reset();

        // count static features for the root move (so branchStats will include them)
        countMoveStatic(moves[i], branchStats);

        rules._board.makeMove(moves[i]);

        countMoveStaticCheck(rules, stats, moves[i]);

        uint64_t nodes = Perft(depth - 1, rules, branchStats);

        rules._board.unmakeMove();

        std::cout << SimpleParser::moveToString(moves[i].OriginSq(), moves[i].TargetSq());
        std::cout << ": " << nodes << "\n";

        stats.nodes += branchStats.nodes;
        stats.captures += branchStats.captures;
        stats.promotions += branchStats.promotions;
        stats.castles += branchStats.castles;
        stats.enPassantCaptures += branchStats.enPassantCaptures;
        stats.checks += branchStats.checks;
        stats.double_checks += branchStats.double_checks;
        stats.discovery_checks += branchStats.discovery_checks;

        total += nodes;
    }

    return total;
}
