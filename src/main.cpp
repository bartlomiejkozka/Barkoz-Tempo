#include "Board.hpp"
#include "PieceMap.hpp"
#include "MoveGeneration/Move.hpp"
#include "MoveGeneration/MoveGenerator.h"
#include "MoveGeneration/ChessRules.hpp"
#include "MoveGeneration/Perft/PerftFunctions.h"
#include "MoveParser.h"


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

    std::string m  = "a2a4";
    std::string m1 = "b7b5"; 
    auto fromTo = std::make_pair(static_cast<int>(SimpleParser::parseMoveString(m).from), static_cast<int>(SimpleParser::parseMoveString(m).to));
    Move mv(fromTo.first, fromTo.second, 0);
    // rules._board.makeMove(mv);
    fromTo = std::make_pair(static_cast<int>(SimpleParser::parseMoveString(m1).from), static_cast<int>(SimpleParser::parseMoveString(m1).to));
    Move mv1(fromTo.first, fromTo.second, 0);
    // rules._board.makeMove(mv1);

    uint64_t nodes = PerftDivide(4, rules, perft_stats);

    perft_stats.print();

    std::cout << "total nodes: " << nodes << std::endl;

    return 0;
}
