// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Class for cheking moves legality/pseudo legality
/*************************************************/
// All piece clear pseudo-legal moves are generated
// in particural piece class.
// "clear pseudo-legal mvoe" - pseudo legal move
// without en passant captures, castling, ...

#ifndef CHESS_RULES_HPP
#define ChESS_RULES_HPP

#include <cstdint>
#include <utility>
#include "Board.hpp"
#include "Move.hpp"

#define MAX_MOVES_NUMBER    (256)   // rael max is 218

// Stateless class, only pure chess mechanics
class ChessRules
{
public:
    // --------------------
    // Castling bit masks
    // --------------------
    static constexpr uint8_t whiteCastlingRightMask = 0x04;
    static constexpr uint8_t blackCastlingRightMask = 0x01;
    static constexpr uint64_t whiteCastlingKingSide  = 0x0000000000000090;
    static constexpr uint64_t whiteCastlingQueenSide = 0x0000000000000011;
    static constexpr uint64_t blackCastlingKingSide  = 0x9000000000000000;
    static constexpr uint64_t blackCastlingQueenSide = 0x1100000000000000;
    // --------------------
    // Initializators
    // --------------------
    ChessRules(Board &board)
        : _board{board} {}

    // --------------------
    // Methods
    // --------------------
    [[nodiscard]] const uint64_t parseAndGetCastlingMoves() const
    {
        std::pair<bool, bool> isCastlingPossible{/*king-site*/0, /*queen-site*/0};
        const std::pair<uint64_t. uint64_t> mask{/*king-site*/0, /*queen-site*/0};
        const uint64_t castlingMap = 0;

        if (PieceDescriptor::nWhite == _board.sideToMove)
        {
            isCastlingPossible = {_board.sideToMove & (whiteCastlingRightMask << 1), 
                                  _board.sideToMove & whiteCastlingRightMask};
            mask = {whiteCastlingKingSide, blackCastlingRightMask};
            castlingMap = _board.bitboards[PieceDescriptor::wKing]
                | _board.bitboards[PieceDescriptor::wRook]
        }
        else if (PieceDescriptor::bWhite == _board.sideToMove)
        {
            isCastlingPossible = {_board.sideToMove & (blackCastlingRightMask << 1), 
                                  _board.sideToMove & blackCastlingRightMask};
            mask = {blackCastlingKingSide, blackCastlingQueenSide};
            castlingMap = _board.bitboards[PieceDescriptor::bKing]
                | _board.bitboards[PieceDescriptor::bRook]        
        }
        else
        {
        }
        
              
    }

private:
    Board &_board;

    // --------------------
    // Methods
    // --------------------
};

#endif
