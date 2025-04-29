#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <cstddef>
#include <cstdint>


/******************************************************************************
 * Every single bitbaord desciptor
 * 6 piece types * 2(white/black) + 2(any pieces by color)
 *******************************************************************************/
enum class PiceDescriptor : size_t
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

    static constexpr size_t bitboardCount = 14;
    

    //==================================
    //===========Board Attibutes========
    //==================================

    std::array<uint64_t, 14> bitboards = {}; //indexed by PiceDescriptor enum
};




#endif