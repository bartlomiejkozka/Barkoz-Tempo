// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Position Evaluation function
/*************************************************/

#include "Evaluation.h"
#include "BitOperation.hpp"
#include "Board.hpp"
#include "MoveGeneration/MoveGenerator.h"

#include <utility>


[[nodiscard]] int Evaluation::Pdiff(Piece piece) const
{
    return count_1s(_rules._board.bitboards[std::to_underlying(piece)])
        - count_1s(_rules._board.bitboards[std::to_underlying(piece) + 1]);
}

[[nodiscard]] int Evaluation::evaluate() const
{
    int materialScore = 
        KingWt   * Pdiff(Piece::King) +
        QueenWt  * Pdiff(Piece::Queen) +
        RookWt   * Pdiff(Piece::Rook) +
        KnightWt * Pdiff(Piece::Knight) +
        BishopWt * Pdiff(Piece::Bishop) +
        PawnWt   * Pdiff(Piece::Pawn);

    // int mobilityScore = 

    return materialScore;
}
