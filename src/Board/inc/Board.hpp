#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <unordered_map>

#include "BitOperation.hpp"


/******************************************************************************
 * Every single bitbaord desciptor
 * 6 piece types * 2(white/black) + 2(any pieces by color)
 *******************************************************************************/
enum class PieceDescriptor : size_t
{
    nWhite,
    nBlack,
    wPawn,
    bPawn,
    wKnight,
    bKnight,
    wBishop,
    bBishop,
    wRook,
    bRook,
    wQueen,
    bQueen,
    wKing,
    bKing
};


// TODO:
// - add castling bitboards
// - add en passant bitboards
// - implement stack of prevMoves to undo moves
// - then could be implemented the useful bit opertaions in signle class


/******************************************************************************
 * Class representing a chess board
 * Main chess fileds: bitboards, playing color, castling, en passant, etc.
 * The board is reprtesented by 14 bitboards, 12 for every cardinal piece type
 * and 2 for any piece of a color (white/black)
 *******************************************************************************/
struct Board
{
    //==================================
    //===========Contructtors===========
    //==================================

    Board()                        = default;
    ~Board()                       = default;
    Board(const Board&)            = default;
    Board &operator=(const Board&) = default;

    //==================================
    //==========Board Predefinitions====
    //==================================

    static constexpr size_t boardSize = 64;
    static constexpr size_t bitboardCount = 14;
    static constexpr size_t enPassantCount = 2; // for white and black pawns
    static constexpr size_t castlingCount = 4; // for white and black kings and rooks

    //==================================
    //======Deafult starting bitboards==
    //==================================

    std::array<uint64_t, 2> defaultKingBitboards = {
        minBitSet << 4, maxBitSet >> 3
    };
    std::array<uint64_t, 2> defaultQueenBitboards = {
        minBitSet << 3, maxBitSet >> 4
    };
    std::array<uint64_t, 2> defaultRookBitboards = {
        minBitSet << 7 | minBitSet,
        maxBitSet >> 7 | maxBitSet
    };
    std::array<uint64_t, 2> defaultBishopBitboards = {
        minBitSet << 5 | minBitSet << 2,
        maxBitSet >> 5 | maxBitSet >> 2
    };
    std::array<uint64_t, 2> defaultKnightBitboards = {
        minBitSet << 6 | minBitSet << 1,
        maxBitSet >> 6 | maxBitSet >> 1
    };
    std::array<uint64_t, 2> defaultPawnBitboards = {
        0xFFUL << 8, 0xFFUL << convertFromMinBitSetToMaxBitSet(16)
    };
    
    //==================================
    //===========Board Attibutes========
    //==================================

    /*
        Bit boards are represented by little endian convenction.
        That means the bottom left corner of board is 0th bit so the least siginificant bit.
        It is also the LSF (Least siginificant file) representation thus the first iteration of bitboard bits is by ranks in these ranks by files.
    */
    std::array<uint64_t, bitboardCount> bitboards = {}; //indexed by PieceDescriptor enum
    uint64_t zobristHash = 0;
    uint8_t sideToMove = static_cast<uint8_t>(PieceDescriptor::nWhite);

    std::unordered_map<uint64_t, uint8_t> repetitions = {}; // hash -> count
    uint8_t halfMoveClock = 0;
    uint8_t enPassant = 0; // enPassant Square, 255 - if no enPassant -> color is not imprtant beacuse of inremental semantic
    uint8_t castlingRights = 0x0F; // 0b00001(white kingsite)1(white queensite)1(black kingsite)1(balck queensite)
};


#endif