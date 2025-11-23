#include "Board.hpp"
#include "PieceMap.hpp"
#include "MoveGeneration/Move.hpp"
#include "MoveGeneration/MoveGenerator.h"
#include "MoveGeneration/ChessRules.hpp"

#include <array>
#include <iostream>

uint64_t Perft(int depth, ChessRules &rules)
{
    std::array<Move, 256> move_list;
    int n_moves;
    int i;
    uint64_t nodes = 0;

    if (depth == 0)
        return 1ULL;

    n_moves = MoveGen::generateLegalMoves(rules, move_list.data());
    for (i = 0; i < n_moves; i++) 
    {
        rules._board.makeMove(move_list[i]);
        nodes += Perft(depth - 1, rules);
        rules._board.unmakeMove();
    }
    return nodes;
}

int main()
{
    PieceMap::init();
    
    Board board{};
    ChessRules rules{board};

    board.init();

    uint64_t nodes = Perft(3, rules);

    std::cout << nodes << std::endl;


    return 0;
}
