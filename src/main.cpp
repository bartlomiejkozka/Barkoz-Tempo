#include "Board.hpp"
#include "PieceMap.hpp"
#include "MoveGeneration/Move.hpp"
#include "MoveGeneration/MoveGenerator.h"
#include "MoveGeneration/ChessRules.hpp"
#include "MoveGeneration/Perft/PerftFunctions.h"
#include "MoveParser.h"
#include "Search/Search.h"
#include "Evaluation/Evaluation.h"

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
    Evaluation::init(rules);

    // board.loadFromFEN("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");

    std::string m  = "c4c5";
    std::string m1 = "e8e7";
    std::string m2 = "c5b6";
    std::string m3 = "c7c5";
    std::string m4 = "e2e4";
    std::string m5 = "g7g5";

    auto fromTo = std::make_pair(static_cast<int>(SimpleParser::parseMoveString(m).from), static_cast<int>(SimpleParser::parseMoveString(m).to));
    Move mv(fromTo.first, fromTo.second, MoveType::QUIET);
    // rules._board.makeMove(mv);
    fromTo = std::make_pair(static_cast<int>(SimpleParser::parseMoveString(m1).from), static_cast<int>(SimpleParser::parseMoveString(m1).to));
    Move mv1(fromTo.first, fromTo.second, MoveType::QUIET);
    // rules._board.makeMove(mv1);
    fromTo = std::make_pair(static_cast<int>(SimpleParser::parseMoveString(m2).from), static_cast<int>(SimpleParser::parseMoveString(m2).to));
    Move mv2(fromTo.first, fromTo.second, MoveType::CAPTURE);
    // rules._board.makeMove(mv2);
    fromTo = std::make_pair(static_cast<int>(SimpleParser::parseMoveString(m3).from), static_cast<int>(SimpleParser::parseMoveString(m3).to));
    Move mv3(fromTo.first, fromTo.second, MoveType::DOUBLE_PUSH);
    // rules._board.makeMove(mv3);
    fromTo = std::make_pair(static_cast<int>(SimpleParser::parseMoveString(m4).from), static_cast<int>(SimpleParser::parseMoveString(m4).to));
    Move mv4(fromTo.first, fromTo.second, MoveType::DOUBLE_PUSH);
    // rules._board.makeMove(mv4);
    fromTo = std::make_pair(static_cast<int>(SimpleParser::parseMoveString(m5).from), static_cast<int>(SimpleParser::parseMoveString(m5).to));
    Move mv5(fromTo.first, fromTo.second, 0);
    // rules._board.makeMove(mv5);

    // uint64_t nodes = PerftDivide(5, rules, perft_stats);
    // perft_stats.print();
    // std::cout << "total nodes: " << nodes << std::endl;

    SearchDivideMinimax(7, rules);

    return 0;
}
