#include <array>
#include <cstdint>
#include <iostream>

#include "Board.hpp"
#include "StateInfo.hpp"
#include "PieceMap.hpp"
#include "BlackPawnMap.hpp"

#include "HashMagicFunctions.hpp"
#include "RookHolder.hpp"
#include "BishopHolder.hpp"


int main()
{
    // Board testBoard;
    // StateInfo_t testStateInfo;
    // std::array<uint64_t, Board::boardSize> bitBoards = {};
    // PieceMap pieceMap;
    // uint64_t hash = pieceMap.generatePosHash(bitBoards);

    //-------
    // Try magic gen for rook
    auto rook = std::make_unique<RookHolder>();
    FancyMagicFunction rookA1Magic = FancyMagicFunction(0, 12, rook.get());
    std::cout << "Magic: " << rookA1Magic.getMagic() << "\nOffset: " << rookA1Magic.getOffset() << std::endl;

    FancyMagicFunction rookH8Magic = FancyMagicFunction(63, 12, rook.get());
    std::cout << "Magic: " << rookH8Magic.getMagic() << "\nOffset: " << rookH8Magic.getOffset() << std::endl;

    // Try magic gen for bishop
    auto bishop = std::make_unique<BishopHolder>();
    FancyMagicFunction bishopB1Magic = FancyMagicFunction(1, 5, bishop.get());
    std::cout << "Magic: " << bishopB1Magic.getMagic() << "\nOffset: " << bishopB1Magic.getOffset() << std::endl;


    return 0;
}
