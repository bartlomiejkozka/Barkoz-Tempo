#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <unordered_map>

#include "BitOperation.hpp"
#include "Move.hpp"


/******************************************************************************
 * Every single bitbaord desciptor
 * 6 piece types * 2(white/black) + 2(any pieces by color)
 *******************************************************************************/
enum class PieceDescriptor : size_t
{
    nWhite, // side to move color indicator
    nBlack, // side to move color indicator
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
* Color indistinguishable PieceType
* (used in Helper: bb(PieceType))
 *******************************************************************************/
enum class Piece : size_t
{
    Pawn   = 2,
    Knight = 4,
    Bishop = 6,
    Rook   = 8,
    Queen  = 10,
    King   = 12
};

enum class pColor : int
{
    White,
    Black
}


// TODO:
// - add castling bitboards
// - add en passant bitboards
// - implement stack of prevMoves to undo moves


/******************************************************************************
 * Class representing a chess board
 * Main chess fileds: bitboards, playing color, castling, en passant, etc.
 * The board is reprtesented by 14 bitboards, 12 for every cardinal piece type
 * and 2 for any piece of a color (white - bottom ranks / black - upper ranks)
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

    static constexpr size_t MAXMoveHistory = 256;

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
    
    // ---------------------------------
    // getters
    // ---------------------------------

    const uint64_t fullBoard() const 
    { 
        return bitboards[PieceDescriptor::nWhite] & bitboards[PieceDescriptor::nBlack]; 
    }

    //==================================
    //===========Board Attibutes========
    //==================================

    /*
        Bit boards are represented by little endian convenction.
        That means the bottom left corner of the board is 0th bit so the least siginificant bit. -> 0b'h8....a1'
        It is also the LSF (Least siginificant file) representation thus the first iteration of bitboard bits is by ranks in these ranks by files.
    */
    std::array<uint64_t, bitboardCount> bitboards = {}; //indexed by PieceDescriptor enum
    uint64_t zobristHash = 0;
    pColor sideToMove = pColor::White;
    std::unordered_map<uint64_t, uint8_t> repetitions = {}; // hash -> count

    // ---------------------------------
    // irreversible game attributes
    // ---------------------------------

    uint8_t halfMoveClock = 0;
    int8_t enPassant = -1;         // enPassant Square, -1 - if no enPassant
    uint8_t castlingRights = 0x0F; // 0b00001(white kingside)1(white queenside)1(black kingside)1(balck queenside)

    std::array<Undo, MAXMoveHistory> history;

    // ---------------------------------
    // Move make
    // ---------------------------------

    void makeMove(Move &m);
    void unmakeMove();

    // ---------------------------------
    // Move make Helpers
    // ---------------------------------

    const size_t getBitboard(const int sq) const;
    void updateOriginBirboard(const int originSq, const int targetSq, const size_t bbN);
    const int calcOpp() { return sideToMove ? -1 : 1; }

    // ---------------------------------
    // Helpers
    // ---------------------------------

    uint64_t bbUs() const
    {
        return bitboards[static_cast<size_t>(sideToMove)];
    }
    uint64_t bbUsRQ() const
    {
        return bitboards[static_cast<size_t>(PieceDescriptor::wRook) + static_cast<size_t>(sideToMove)]
            | bitboards[static_cast<size_t>(PieceDescriptor::wQueen) + static_cast<size_t>(sideToMove)];
    }
    uint64_t bbUsBQ() const
    {
        return bitboards[static_cast<size_t>(PieceDescriptor::wBishop) + static_cast<size_t>(sideToMove)]
            | bitboards[static_cast<size_t>(PieceDescriptor::wQueen) + static_cast<size_t>(sideToMove)];
    }

    uint64_t bbThem() const
    {
        return static_cast<bool>(sideToMove) ? bitboards[static_cast<size_t>(sideToMove) - 1] : bitboards[static_cast<size_t>(sideToMove) + 1];
    }
    uint64_t bbThemRQ() const
    {
        return bitboards[static_cast<size_t>(PieceDescriptor::bRook) - static_cast<size_t>(sideToMove)]
            | bitboards[static_cast<size_t>(PieceDescriptor::bQueen) - static_cast<size_t>(sideToMove)];
    }
    uint64_t bbThemBQ() const
    {
        return bitboards[static_cast<size_t>(PieceDescriptor::bBishop) - static_cast<size_t>(sideToMove)]
            | bitboards[static_cast<size_t>(PieceDescriptor::bQueen) - static_cast<size_t>(sideToMove)];
    }
    uint64_t bbThemSliders() const
    {
        return bitboards[static_cast<size_t>(PieceDescriptor::bBishop) - static_cast<size_t>(sideToMove)]
            | bitboards[static_cast<size_t>(PieceDescriptor::bQueen) - static_cast<size_t>(sideToMove)]
            | bitboards[static_cast<size_t>(PieceDescriptor::bRook) - static_cast<size_t>(sideToMove)];
    }


    uint64_t bbUs(Piece pieceType) const
    {
        return bitboards[static_cast<size_t>(pieceType) + static_cast<size_t>(sideToMove)] 
    }

    uint64_t bbThem(Piece pieceType) const
    {
        return bitboards[static_cast<size_t>(pieceType) + 1 - static_cast<size_t>(sideToMove)]
    }
};


#endif
