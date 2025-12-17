// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Position Evaluation function
/*************************************************/

#include "Evaluation.h"
#include "BitOperation.hpp"
#include "MoveGeneration/ChessRules.hpp"
#include "MoveGeneration/MoveGenerator.h"
#include "Board.hpp"

#include <utility>
#include <bit>
#include <array>


// ------- PST ----------
namespace 
{
    // distninct Piece Count
    constexpr int pDistinct = 6;

    constexpr int PawnPhase = 0;
    constexpr int KnightPhase = 1;
    constexpr int BishopPhase = 1;
    constexpr int RookPhase = 2;
    constexpr int QueenPhase = 4;
    constexpr int TotalPhase = (4*KnightPhase) + (4*BishopPhase) +
                               (4*RookPhase) + (2*QueenPhase);

    constexpr std::array<int, pDistinct> PhaseTab =
    {
        PawnPhase, KnightPhase, BishopPhase,
        RookPhase, QueenPhase, 0 // 0 - King
    };

    constexpr std::array<int, pDistinct> mgMaterialWeightTab =
    {
        Evaluation::PawnWt, Evaluation::KnightWt+40, Evaluation::BishopWt+70,
        Evaluation::RookWt-30, Evaluation::QueenWt+100, 0 /*Evaluation::KingWt*/
    };
    constexpr std::array<int, pDistinct> egMaterialWeightTab =
    {
        Evaluation::PawnWt, Evaluation::KnightWt-20, Evaluation::BishopWt-10,
        Evaluation::RookWt+10, Evaluation::QueenWt, 0 /*Evaluation::KingWt*/
    };

    int getPSTEval(int opening, int endgame, int phase)
    {
        return ( (opening * (256 - phase)) + (endgame * phase) ) / 256;
    }

    std::array<std::array<int, Board::boardSize>, Board::bitboardCount-2> mgTab{};
    std::array<std::array<int, Board::boardSize>, Board::bitboardCount-2> egTab{};

    void initPST();
}


[[nodiscard]] int Evaluation::evaluate(ChessRules &rules)
{
    int positionalAndMaterialScore = 0;

    int currentPhase = TotalPhase;
    std::array<int, 2> mg{};
    std::array<int, 2> eg{};

    for (int i = 0; i < pDistinct; ++i) 
    {
        auto bbIdx = static_cast<size_t>((2 * i) + 2);

        std::array<int, 2> piecesCount{};
        for (int color = 0; color < 2; ++color)
        {
            uint64_t bb = rules._board.bitboards[bbIdx + color];

            while (bb) 
            {
                int sq = pop_1st(bb);
                mg[color] += mgTab[bbIdx-2][sq];
                eg[color] += egTab[bbIdx-2][sq];
                currentPhase -= PhaseTab[i];
                piecesCount[color]++;
            }
        }
    }

    currentPhase = (currentPhase * 256 + (TotalPhase / 2)) / TotalPhase;
    int opening = mg[0] - mg[1];
    int endgame = eg[0] - eg[1];
    positionalAndMaterialScore = getPSTEval(opening, endgame, currentPhase);


    auto getMobilityFor = [&rules](pColor color) {
        rules._board.sideToMove = color;
        std::array<Move, 256> dummyMoves;
        int mobilityScore = 0;
        MoveGen::generateLegalMoves(rules, dummyMoves.data(), &mobilityScore, Evaluation::MobilityWeights.data());
        return mobilityScore;
    };

    const pColor originalSide = rules._board.sideToMove;
    int whiteMobility = getMobilityFor(pColor::White);
    int blackMobility = getMobilityFor(pColor::Black);
    rules._board.sideToMove = originalSide;
    
    int mobilityScore = whiteMobility - blackMobility;

    return (positionalAndMaterialScore + mobilityScore);
}

void Evaluation::init()
{
    initPST();
}

[[nodiscard]] int Evaluation::getPieceValue(PieceDescriptor piece)
{
    switch (piece)
    {
        case PieceDescriptor::wPawn:
        case PieceDescriptor::bPawn:
            return PawnWt;
        case PieceDescriptor::wKnight:
        case PieceDescriptor::bKnight:
            return KnightWt;
        case PieceDescriptor::wBishop:
        case PieceDescriptor::bBishop:
            return BishopWt;
        case PieceDescriptor::wRook:
        case PieceDescriptor::bRook:
            return RookWt;
        case PieceDescriptor::wQueen:
        case PieceDescriptor::bQueen:
            return QueenWt;
        case PieceDescriptor::wKing:
        case PieceDescriptor::bKing:
            return KingWt;
        default:
        {
            //std::unreachable();
            return 0;
        }
    }
}


// ------- PST ----------
namespace 
{
    constexpr std::array<int, 64> mgPawnTable = 
    {
        // all initial position - third, fourth rank +20pkt in two middle pawns
        0,   0,   0,   0,   0,   0,  0,   0,
        98, 134,  61,  95,  68, 126, 34, -11,
        -6,   7,  26,  71,  95,  56, 25, -20,
        -14,  13,   6,  61,  63,  12, 17, -23,
        -27,  -2,  -5,  12,  17,   6, 10, -25,
        -26,  -4,  -4, -10,   3,   3, 33, -12,
        -35,  -1, -20, -23, -15,  24, 38, -22,
        0,   0,   0,   0,   0,   0,  0,   0,
    };
    constexpr std::array<int, 64> egPawnTable = 
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        178, 173, 158, 134, 147, 132, 165, 187,
        94, 100,  85,  67,  56,  53,  82,  84,
        32,  24,  13,   5,  -2,   4,  17,  17,
        13,   9,  -3,  -7,  -7,  -8,   3,  -1,
        4,   7,  -6,   1,   0,  -5,  -1,  -8,
        13,   8,   8,  10,  13,   0,   2,  -7,
        0,   0,   0,   0,   0,   0,   0,   0,
    };

    constexpr std::array<int, 64> mgKnightTable = 
    {
        -167, -89, -34, -49,  61, -97, -15, -107,
        -73, -41,  72,  36,  23,  62,   7,  -17,
        -47,  60,  37,  65,  84, 129,  73,   44,
        -9,  17,  19,  53,  37,  69,  18,   22,
        -13,   4,  16,  13,  28,  19,  21,   -8,
        -23,  -9,  12,  10,  19,  17,  25,  -16,
        -29, -53, -12,  -3,  -1,  18, -14,  -19,
        -105, -21, -58, -33, -17, -28, -19,  -23,
    };
    constexpr std::array<int, 64> egKnightTable = 
    {
        -58, -38, -13, -28, -31, -27, -63, -99,
        -25,  -8, -25,  -2,  -9, -25, -24, -52,
        -24, -20,  10,   9,  -1,  -9, -19, -41,
        -17,   3,  22,  22,  22,  11,   8, -18,
        -18,  -6,  16,  25,  16,  17,   4, -18,
        -23,  -3,  -1,  15,  10,  -3, -20, -22,
        -42, -20, -10,  -5,  -2, -20, -23, -44,
        -29, -51, -23, -15, -22, -18, -50, -64,
    };

    constexpr std::array<int, 64> mgBishopTable = 
    {
        // modified right bishop initial diagonal all squares -70pkt - right bishop
        // modified right bishop initial diagonal all squares -30pkt - left bishop
        -29,   4, -82, -37, -25, -42,   7,  -8,
        -26,  16, -18, -43,  -40,  59,  18, -47,
        -16,  37,  43,  -30,  5,  50,  37,  -2,
        -4,   5,  -51,  50,  37,  7,   7,  -2,
        -6,  -57,  13,  26,  34,  12,  -20,   4,
        -70,  15,  15,  15,  14,  27,  18,  -20,
        4,  15,  16,   0,   7,  21,  33,   1,
        -33,  -3, -14, -21, -13, -12, -39, -21,
    };
    constexpr std::array<int, 64> egBishopTable = 
    {
        -14, -21, -11,  -8, -7,  -9, -17, -24,
        -8,  -4,   7, -12, -3, -13,  -4, -14,
        2,  -8,   0,  -1, -2,   6,   0,   4,
        -3,   9,  12,   9, 14,  10,   3,   2,
        -6,   3,  13,  19,  7,  10,  -3,  -9,
        -12,  -3,   8,  10, 13,   3,  -7, -15,
        -14, -18,  -7,  -1,  4,  -9, -15, -27,
        -23,  -9, -23,  -5, -9, -16,  -5, -17,
    };

    constexpr std::array<int, 64> mgRookTable = 
    {
        32,  42,  32,  51, 63,  9,  31,  43,
        27,  32,  58,  62, 80, 67,  26,  44,
        -5,  19,  26,  36, 17, 45,  61,  16,
        -24, -11,   7,  26, 24, 35,  -8, -20,
        -36, -26, -12,  -1,  9, -7,   6, -23,
        -45, -25, -16, -17,  3,  0,  -5, -33,
        -44, -16, -20,  -9, -1, 11,  -6, -71,
        -19, -13,   1,  17, 16,  7, -37, -26,
    };
    constexpr std::array<int, 64> egRookTable = 
    {
        13, 10, 18, 15, 12,  12,   8,   5,
        11, 13, 13, 11, -3,   3,   8,   3,
        7,  7,  7,  5,  4,  -3,  -5,  -3,
        4,  3, 13,  1,  2,   1,  -1,   2,
        3,  5,  8,  4, -5,  -6,  -8, -11,
        -4,  0, -5, -1, -7, -12,  -8, -16,
        -6, -6,  0,  2, -9,  -9, -11,  -3,
        -9,  2,  3, -1, -5, -13,   4, -20,
    };

    constexpr std::array<int, 64> mgQueenTable = 
    {
        -28,   0,  29,  12,  59,  44,  43,  45,
        -24, -39,  -5,   1, -36,  57,  28,  54, // to -36
        -13, -17,   7,   8,  29,  -4,  47,  57, // modified pre-pre-last value from 56 to -4
        -27, -27, -16, -16,  -1,  17,  -49,   1, // modified pre-last value from -2 to -49
        -9, -26,  -9, -10,  -2,  -4,   3,  -70, // modified last value from -3 to -70
        -14,   2, -11,  -2,  -5,   2,  14,   5,
        -35,  -8,  11,   2,   8,  15,  -3,   1,
        -1, -18,  -9,  10, -15, -25, -31, -50,
    };
    constexpr std::array<int, 64> egQueenTable = 
    {
        -9,  22,  22,  27,  27,  19,  10,  20,
        -17,  20,  32,  41,  58,  25,  30,   0,
        -20,   6,   9,  49,  47,  35,  19,   9,
        3,  22,  24,  45,  57,  40,  57,  36,
        -18,  28,  19,  47,  31,  34,  39,  23,
        -16, -27,  15,   6,   9,  17,  10,   5,
        -22, -23, -30, -16, -16, -23, -36, -32,
        -33, -28, -22, -43,  -5, -32, -20, -41,
    };

    constexpr std::array<int, 64> mgKingTable = 
    {
        -65,  23,  16, -15, -56, -34,   2,  13,
        29,  -1, -20,  -7,  -8,  -4, -38, -29,
        -9,  24,   2, -16, -20,   6,  22, -22,
        -17, -20, -12, -27, -30, -25, -14, -36,
        -49,  -1, -27, -39, -46, -44, -33, -51,
        -14, -14, -22, -46, -44, -30, -15, -27,
        1,   7,  -8, -64, -43, -16,   9,   8,
        -15,  36,  12, -54,   8, -28,  24,  14,
    };
    constexpr std::array<int, 64> egKingTable = 
    {
        -74, -35, -18, -18, -11,  15,   4, -17,
        -12,  17,  14,  17,  17,  38,  23,  11,
        10,  17,  23,  15,  20,  45,  44,  13,
        -8,  22,  24,  27,  26,  33,  26,   3,
        -18,  -4,  21,  24,  27,  23,   9, -11,
        -19,  -3,  11,  21,  23,  16,   7,  -9,
        -27, -11,   4,  13,  14,   4,  -5, -17,
        -53, -34, -21, -11, -28, -14, -24, -43
    };

    constexpr std::array<const int*, pDistinct> mgPstTab =
    {
        mgPawnTable.data(), mgKnightTable.data(), mgBishopTable.data(),
        mgRookTable.data(), mgQueenTable.data(), mgKingTable.data()
    };
    constexpr std::array<const int*, pDistinct> egPstTab =
    {
        egPawnTable.data(), egKnightTable.data(), egBishopTable.data(),
        egRookTable.data(), egQueenTable.data(), egKingTable.data()
    };

    // by XORing square value number by 56 which is 111000 in binary, we get fliped PST tables
    void initPST()
    {
        for (int p = 0, bbIdx = 0; bbIdx < Board::bitboardCount-2; bbIdx += 2, ++p)
        {
            for (int sq = 0; sq < Board::boardSize; ++sq)
            {
                // for white we have to flip PST tables because of our board repr. (A1=0, h8=63)
                mgTab[bbIdx][sq] = (mgPstTab[p][sq^56]/3) + mgMaterialWeightTab[p];
                egTab[bbIdx][sq] = (egPstTab[p][sq^56]/3) + egMaterialWeightTab[p];

                mgTab[bbIdx+1][sq] = (mgPstTab[p][sq]/3) + mgMaterialWeightTab[p];
                egTab[bbIdx+1][sq] = (egPstTab[p][sq]/3) + egMaterialWeightTab[p];
            }
        }
    }
}
