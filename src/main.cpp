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

    board.loadFromFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0");

    std::string m  = "h1g1";
    std::string m1 = "g7h6";
    std::string m2 = "e2e3";
    std::string m3 = "c6d4";
    std::string m4 = "f1c4";
    std::string m5 = "g7g5";

    auto fromTo = std::make_pair(static_cast<int>(SimpleParser::parseMoveString(m).from), static_cast<int>(SimpleParser::parseMoveString(m).to));
    Move mv(fromTo.first, fromTo.second, MoveType::QUIET);
    // rules._board.makeMove(mv);
    fromTo = std::make_pair(static_cast<int>(SimpleParser::parseMoveString(m1).from), static_cast<int>(SimpleParser::parseMoveString(m1).to));
    Move mv1(fromTo.first, fromTo.second, MoveType::QUIET);
    // rules._board.makeMove(mv1);
    fromTo = std::make_pair(static_cast<int>(SimpleParser::parseMoveString(m2).from), static_cast<int>(SimpleParser::parseMoveString(m2).to));
    Move mv2(fromTo.first, fromTo.second, 0);
    // rules._board.makeMove(mv2);
    fromTo = std::make_pair(static_cast<int>(SimpleParser::parseMoveString(m3).from), static_cast<int>(SimpleParser::parseMoveString(m3).to));
    // Move mv3(fromTo.first, fromTo.second, 1);
    // rules._board.makeMove(mv3);
    fromTo = std::make_pair(static_cast<int>(SimpleParser::parseMoveString(m4).from), static_cast<int>(SimpleParser::parseMoveString(m4).to));
    Move mv4(fromTo.first, fromTo.second, 0);
    // rules._board.makeMove(mv4);
    fromTo = std::make_pair(static_cast<int>(SimpleParser::parseMoveString(m5).from), static_cast<int>(SimpleParser::parseMoveString(m5).to));
    Move mv5(fromTo.first, fromTo.second, 0);
    // rules._board.makeMove(mv5);

    uint64_t nodes = PerftDivide(4, rules, perft_stats);

    perft_stats.print();

    std::cout << "total nodes: " << nodes << std::endl;

    return 0;
}

// 24, 25