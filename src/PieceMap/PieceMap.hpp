#include "Board.hpp"

#include <stdint.h>
#include <array>
#include <unordered_set>
#include <random>
#include <bit>

// For now only stores initial hashes for pieces
// Then check the transposition table as related !!!

/******************************************************************************
 * Class representing a Zobrist hash table
 * The Zobrist hash table is used to store the hash values of any piece
 * on any square of the board.
 *******************************************************************************/

struct PieceMap
{
    //----------Zobrsit hash tabele----------
    std::array<std::array<uint64_t, Board::boardSize>, 12> pieceMap = [] () {
        std::array<std::array<uint64_t, Board::boardSize>, 12> pieceMap;
        std::mt19937_64 rng(416587);
        std::uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);

        std::unordered_set<uint64_t> uniqueHashes;
        for (size_t i = 0; i < pieceMap.size(); ++i)
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
    
    //------------Constructors-------------
    PieceMap() = default;
    ~PieceMap() = default;
    PieceMap(const PieceMap&) = default;
    PieceMap& operator=(const PieceMap&) = default;

    //------------Methods-----------------
    const uint64_t generatePosHash(const std::array<uint64_t, Board::boardSize>& bitBoards)
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

        return posHash;
    }
};
