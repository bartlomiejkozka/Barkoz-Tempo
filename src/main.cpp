#include "Board.hpp"
#include "PieceMap.hpp"
#include "MoveGeneration/Move.hpp"
#include "MoveGeneration/MoveGenerator.h"
#include "MoveGeneration/ChessRules.hpp"
#include "MoveGeneration/Perft/PerftFunctions.h"
#include "MoveParser.h"
#include "Engine/TranspositionTable.h"
#include "Engine/Search.h"
#include "Engine/Evaluation.h"
#include "Uci/Uci.h"

#include <array>
#include <iostream>
#include <utility>

int main()
{
    PieceMap::init();
    Board board{};
    PerftStats perft_stats{};
    ChessRules rules{board, perft_stats};
    board.init();
    Evaluation::init();

    auto TT = TranspositionTable();
    auto searchEngine = Search(TT);

    auto engine = UCI(rules, searchEngine);

    engine.loop();

    return 0;
}
