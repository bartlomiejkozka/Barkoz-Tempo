#include "Board.hpp"
#include "PieceMap.hpp"

int main()
{
    PieceMap::init();
    Board board{};
 
    board.init();


    return 0;
}
