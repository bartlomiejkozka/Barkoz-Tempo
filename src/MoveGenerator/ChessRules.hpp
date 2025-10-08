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

#include "Board.hpp"
#include "Move.hpp"

#define MAX_MOVES_NUMBER    (256)   // rael max is 218

class ChessRules 
{
private:
    Board &_board;
    
public:
    //--------------------
    // Initializators
    //--------------------
    ChessRules(Board &board)
        : _board{board} {}

    //--------------------
    // Methods
    //--------------------
    [[nodiscard]] 

    // generate moves list without check the check
    template<PieceDescriptor Piece_t>
    [[nodiscard]] std:array<Move, MAX_MOVES_NUMBER> getPseudoLegalMoves()
    {
        for (uint64_t bb = _board.bitboards[Piece_t]; 0 != bb; bb &= bb - 1)
        {
             
        }
    }
};

#endif
