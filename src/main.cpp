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

    std::cout << "ppp" << std::endl;
    for (Move* p = moves.data(); p < ptr; ++p) 
    {
        std::cout << p->OriginSq() << "\n";
    }

    return 0;
}
