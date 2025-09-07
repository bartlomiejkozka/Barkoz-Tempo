#include <array>
#include <cstdint>
#include <iostream>

#include "Board.hpp"
#include "StateInfo.hpp"
#include "PieceMap.hpp"
#include "BlackPawnMap.hpp"
#include "HashMagicFunctions.hpp"


int main()
{
    // Board testBoard;
    // StateInfo_t testStateInfo;
    // std::array<uint64_t, Board::boardSize> bitBoards = {};
    // PieceMap pieceMap;
    // uint64_t hash = pieceMap.generatePosHash(bitBoards);

    //-------
    // Try magic gen for rook
    FancyMagicFunction rookA1Magic = FancyMagicFunction(0, 12, MagicHolder::Rook);
    std::cout << "Magic: " << rookA1Magic.getMagic() << "\nOffset: " << rookA1Magic.getOffset() << std::endl;

    return 0;
}
