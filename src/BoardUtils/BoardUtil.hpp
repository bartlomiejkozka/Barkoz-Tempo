#ifndef BOARD_UTIL_HPP
#define BOARD_UTIL_HPP

#include "Board.hpp"


struct PerformanceTests
{
    PerformanceTests(const Board& board) : board(board) {}
    ~PerformanceTests() = default;

    //========Perft Tests Attrubutes========
    uint64_t captures = 0;
    uint64_t enPassant = 0;
    uint64_t castles = 0;
    uint64_t promotions = 0;
    uint64_t checks = 0;
    uint64_t discoveryChecks = 0;
    uint64_t doubleChecks = 0;
    uint64_t checkmates = 0;

    Board board;
};


#endif