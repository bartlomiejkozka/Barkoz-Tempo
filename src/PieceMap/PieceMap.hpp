#ifndef ZOBRIST_HASH_HPP
#define ZOBRIST_HASH_HPP

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
    static constexpr int pieceMapsCount = Board::bitboardCount - 2;
    static constexpr int castlingRighstCount = 4;
    static constexpr int enPassantFilesCount = 8;

    //----------Zobrsit hash tabele----------

    // One number for each piece at each square 
    static std::array<std::array<uint64_t, Board::boardSize>, pieceMapsCount> pieceMap;
    
    // One number to indicate the side to move is black
    static uint64_t blackSideToMove;

    // Four numbers to indicate the castling rights, though usually 16 (2^4) are used for speed
    static std::array<uint64_t, castlingRighstCount> castlingRightsMap;   // WK, WQ, BK, BQ - revers

    // Eight numbers to indicate the file of a valid En passant square, if any
    static std::array<uint64_t, enPassantFilesCount> enPassantsMap;   // A -> H file

    // ------------------------
    // Initialization
    // ------------------------

    static void init();

    // ------------------------
    // Generation
    // ------------------------

    const uint64_t generatePosHash(const std::array<uint64_t, Board::boardSize>& bitBoards, Board &board);
};

#endif
