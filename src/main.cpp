#include "Board.hpp"
#include "PieceMap.hpp"
#include "MoveGeneration/Move.hpp"
#include "MoveGeneration/MoveGenerator.h"
#include "MoveGeneration/ChessRules.hpp"
#include "MoveGeneration/Perft/PerftFunctions.h"
#include "MoveParser.h"
#include "Engine/TranspositionTable.h"
#include "Engine/Search.h"
#include "Uci/Uci.h"

#include <array>
#include <iostream>
#include <utility>


int main()
{
    // board.loadFromFEN("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");

    PieceMap::init();
    Board board{};
    PerftStats perft_stats{};
    ChessRules rules{board, perft_stats};
    board.init();
    // Evaluation::init(rules);

    auto TT = TranspositionTable();
    auto searchEngine = Search(TT);

    auto engine = UCI(rules, searchEngine);

    engine.loop();

    return 0;
}
