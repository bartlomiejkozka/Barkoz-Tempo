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
    uint64_t nodes = 0;

    if (depth == 0)
        return 1ULL;

    n_moves = MoveGen::generateLegalMoves(rules, move_list.data());
    for (int i = 0; i < n_moves; i++) 
    {
        rules._board.makeMove(move_list[i]);
        nodes += Perft(depth - 1, rules);
        rules._board.unmakeMove();
    }
    return nodes;
}

uint64_t perft_divide(int depth, ChessRules &rules) {
    std::array<Move, 256> moves;
    int moveCount = MoveGen::generateLegalMoves(rules, moves.data());

    uint64_t total = 0;

    for (int i = 0; i < moveCount; i++)
    {
        rules._board.makeMove(moves[i]);
        uint64_t nodes = Perft(depth - 1, rules);
        rules._board.unmakeMove();

        std::cout << moves[i].OriginSq() << " -> " << moves[i].TargetSq();
        std::cout << ": " << nodes << "\n";

        total += nodes;
    }

    return total;
}

int main()
{
    PieceMap::init();
    
    Board board{};
    ChessRules rules{board};

    board.init();

    uint64_t nodes = perft_divide(3, rules);

    std::cout << "total: " << nodes << std::endl;

    return 0;
}
