#include "Board.hpp"
#include "PieceMap.hpp"
#include "MoveGeneration/Move.hpp"
#include "MoveGeneration/MoveGenerator.h"
#include "MoveGeneration/ChessRules.hpp"

#include <array>
#include <iostream>

int main()
{
    PieceMap::init();
    
    Board board{};
    ChessRules rules{board};

    board.init();

    std::array<Move, 256> moves{};
    Move *ptr = moves.data();
    ptr = MoveGen::generateLegalMoves(rules, ptr);

    board.makeMove(moves[0]);

    std::array<Move, 256> moves2{};
    Move *ptr2 = moves2.data();
    ptr2 = MoveGen::generateLegalMoves(rules, ptr2);

    for (Move* p = moves2.data(); p < ptr2; ++p) 
    {
        std::cout << p->OriginSq() << " -> " << p->TargetSq() << "\n";
    }

    return 0;
}
