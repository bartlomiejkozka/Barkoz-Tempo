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

    std::string m  = "d2d3";
    std::string m1 = "c7c6"; 
    std::string m2 = "e1d2";
    std::string m3 = "d8a5";
    auto fromTo = std::make_pair(static_cast<int>(SimpleParser::parseMoveString(m).from), static_cast<int>(SimpleParser::parseMoveString(m).to));
    Move mv(fromTo.first, fromTo.second, 0);
    rules._board.makeMove(mv);
    fromTo = std::make_pair(static_cast<int>(SimpleParser::parseMoveString(m1).from), static_cast<int>(SimpleParser::parseMoveString(m1).to));
    Move mv1(fromTo.first, fromTo.second, 0);
    rules._board.makeMove(mv1);
    fromTo = std::make_pair(static_cast<int>(SimpleParser::parseMoveString(m2).from), static_cast<int>(SimpleParser::parseMoveString(m2).to));
    Move mv2(fromTo.first, fromTo.second, 0);
    rules._board.makeMove(mv2);
    fromTo = std::make_pair(static_cast<int>(SimpleParser::parseMoveString(m3).from), static_cast<int>(SimpleParser::parseMoveString(m3).to));
    Move mv3(fromTo.first, fromTo.second, 0);
    rules._board.makeMove(mv3);

    uint64_t nodes = PerftDivide(1, rules, perft_stats);

    perft_stats.print();

    std::cout << "total nodes: " << nodes << std::endl;

    return 0;
}
