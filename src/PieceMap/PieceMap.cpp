// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Class for moves generation - used by ChessRules, which is considered as Higher Level Interface
/*************************************************/

#include "PieceMap.hpp"
#include "BitOperation.hpp"

// have to be here repeated becasue of linker err.
std::array<std::array<uint64_t, Board::boardSize>, PieceMap::pieceMapsCount> PieceMap::pieceMap;
uint64_t PieceMap::blackSideToMove;
std::array<uint64_t, PieceMap::castlingRighstCount> PieceMap::castlingRightsMap;
std::array<uint64_t, PieceMap::enPassantFilesCount> PieceMap::enPassantsMap;

void PieceMap::init()
{
    std::unordered_set<uint64_t> uniqueHashes;
    std::mt19937_64 rng(416587);
    std::uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);

    pieceMap = [&] ()
    {
        std::array<std::array<uint64_t, Board::boardSize>, pieceMapsCount> map;
  
        for (size_t i = 0; i < pieceMapsCount; ++i)
        {
            for (size_t j = 0; j < Board::boardSize; ++j)
            {
                uint64_t rd_num;
                do
                {
                    rd_num = dist(rng);
                } while (!uniqueHashes.insert(rd_num).second);
                map[i][j] = rd_num;
            }
        }

        return map;
    }();

    blackSideToMove = [&] ()
    {
        uint64_t rd_num;
        do
        {
            rd_num = dist(rng);
        } while ( !uniqueHashes.insert(rd_num).second );

        return rd_num;
    }();

    castlingRightsMap = [&] ()
    {
        std::array<uint64_t, castlingRighstCount> res;
        uint64_t rd_num;

        for (size_t i = 0; i < castlingRighstCount; ++i)
        {
            do
            {
                rd_num = dist(rng);
            } while (!uniqueHashes.insert(rd_num).second);
            res[i] = rd_num;
        }

        return res;
    }();

    enPassantsMap = [&] ()
    {
        std::array<uint64_t, enPassantFilesCount> res;
        uint64_t rd_num;

        for (size_t i = 0; i < enPassantFilesCount; ++i)
        {
            do
            {
                rd_num = dist(rng);
            } while (!uniqueHashes.insert(rd_num).second);
            res[i] = rd_num;
        }

        return res;
    }();

    // TODO: Mingw does not see the lambda template, change to MSVC

    // auto genHashMap = [&] <int size> ()
    // {
    //     std::array<uint64_t, size> res;
    //     uint64_t rd_num;

    //     for (size_t i = 0; i < size; ++i)
    //     {
    //         do
    //         {
    //             rd_num = dist(rng);
    //         } while (!uniqueHashes.insert(rd_num).second);
    //         res[i] = rd_num;
    //     }

    //     return res;
    // };

    // castlingRightsMap = genHashMap<castlingRighstCount>();
    // enPassantsMap = genHashMap<enPassantFilesCount>();
}

uint64_t PieceMap::generatePosHash(const Board &b)
{
    uint64_t posHash = 0;
    for (size_t i = 2; i < Board::bitboardCount; ++i)
    {
        uint64_t board = b.bitboards[i];
        while (board)   // till any bit is set
        {
            auto idx = static_cast<size_t>(std::countr_zero(board));
            posHash ^= pieceMap[i][idx];
            board &= (board - 1);
        }
    }

    posHash ^= static_cast<bool>(b.sideToMove) ? blackSideToMove : 0;

    auto cast = static_cast<uint64_t>(b.castlingRights);
    while (cast)
    {
        int idx = pop_1st(cast); // teraz OK, lvalue 64-bit
        posHash ^= castlingRightsMap[idx];
    }

    if ( b.enPassant != -1 )
    {
        posHash ^= enPassantsMap[b.enPassant % 8];
    }

    return posHash;
}
