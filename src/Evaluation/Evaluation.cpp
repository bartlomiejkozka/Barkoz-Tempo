// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Position Evaluation function
/*************************************************/

#include "Evaluation.h"
#include "BitOperation.hpp"
#include "MoveGeneration/ChessRules.hpp"
#include "MoveGeneration/MoveGenerator.h"

#include <utility>


[[nodiscard]] int Evaluation::evaluate(ChessRules &rules)
{
    auto Pdiff = [&rules] (Piece piece)
    {
        return count_1s(rules._board.bitboards[std::to_underlying(piece)])
            - count_1s(rules._board.bitboards[std::to_underlying(piece) + 1]);
    };

    int materialScore = 
        KingWt   * Pdiff(Piece::King) +
        QueenWt  * Pdiff(Piece::Queen) +
        RookWt   * Pdiff(Piece::Rook) +
        KnightWt * Pdiff(Piece::Knight) +
        BishopWt * Pdiff(Piece::Bishop) +
        PawnWt   * Pdiff(Piece::Pawn);

    auto getMobilityFor = [&rules](pColor color) {
        rules._board.sideToMove = color;
        std::array<Move, 256> dummyMoves;
        return MoveGen::generateLegalMoves(rules, dummyMoves.data());
    };

    const pColor originalSide = rules._board.sideToMove;
    int whiteMobility = getMobilityFor(pColor::White);
    int blackMobility = getMobilityFor(pColor::Black);
    rules._board.sideToMove = originalSide;
    
    int mobilityScore = MobilityWt * (whiteMobility - blackMobility);

    return (materialScore + mobilityScore);
        // * (static_cast<bool>(rules._board.sideToMove) ? -1 : 1);
}

void Evaluation::init(ChessRules &rules)
{
    rules._board.currentScore = evaluate(rules);
    rules._board.shortMem[rules._board.ply].score = rules._board.currentScore;
}
