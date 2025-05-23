#include "Board.hpp"
#include "StateInfo.hpp"
#include "PieceMap.hpp"
#include <array>
#include <cstdint>


int main()
{
    Board testBoard;
    StateInfo_t testStateInfo;
    std::array<uint64_t, Board::boardSize> bitBoards = {};
    PieceMap pieceMap;
    uint64_t hash = pieceMap.generatePosHash(bitBoards);

    return 0;
}
