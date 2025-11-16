// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Class for moves generation - used by ChessRules, which is considered as Higher Level Interface
/*************************************************/

#include "PieceMap.hpp"
#include "BitOperation.hpp"

static void PieceMap::init()
{
    std::unordered_set<uint64_t> uniqueHashes;
    std::mt19937_64 rng(416587);
    std::uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);

    pieceMap = [&] ()
    {
        std::array<std::array<uint64_t, Board::boardSize>, pieceMapsCount> pieceMap;
  
        for (size_t i = 0; i < pieceMapsCount; ++i)
        {
            for (size_t j = 0; j < Board::boardSize; ++j)
            {
                uint64_t rd_num;
                do
                {
                    rd_num = dist(rng);
                } while (!uniqueHashes.insert(rd_num).second);
                pieceMap[i][j] = rd_num;
            }
        }

        return pieceMap;
    }();

    blackSideToMove = [&] ()
    {
        do
        {
            uint64_t rd_num = dist(rng);
        } while ( !uniqueHashes.insert(rd_num).second );

        return rd_num;
    }();

    auto genHashMap = [&] ( const int size )
    {
        std::array<uint64_t, size> res;
        uint64_t rd_num;

        for (size_t i = 0; i < size; ++i)
        {
            do
            {
                rd_num = dist(rng);
            } while (!uniqueHashes.insert(rd_num).second);
            res[i] = rd_num;
        }

        return res;
    }    

    castlingRightsMap = genHashMap(castlingRighstCount);

    enPassantsMap = genHashMap(enPassantFilesCount);
}

const uint64_t PieceMap::generatePosHash(const std::array<uint64_t, Board::boardSize>& bitBoards, Board &board)
{
    uint64_t posHash = 0;
    for (size_t i = 2; i < Board::bitboardCount; ++i)
    {
        uint64_t board = bitBoards[i];
        while (board)   // till any bit is set
        {
            uint8_t idx = std::countr_zero(board);
            posHash ^= pieceMap[i][idx];
            board &= (board - 1);
        }
    }

    posHash ^= board.sideToMove ? blackSideToMove : 0;

    for ( uint8_t cast = board.castlingRights; cast != 0; )
    {
        int idx = pop_1st(cast);
        posHash ^= castlingRightsMap[idx];
    }

    if ( board.enPassant != -1 )
    {
        posHash ^= enPassantsMap[board.enPassant % 8];
    }

    return posHash;
}
