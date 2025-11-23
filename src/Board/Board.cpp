// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Game logic holder
/*************************************************/

#include "Board.hpp"
#include "MoveGeneration/MoveUtils.hpp"
#include "PieceMap.hpp"

#include <utility>


// ---------------------------------
// Initilizator
// ---------------------------------

void Board::init()
{
    // Bitboards init
    static const std::array<uint64_t, 2> defaultKingBitboards = {
        minBitSet << 4,
        maxBitSet >> 3
    };
    static const std::array<uint64_t, 2> defaultQueenBitboards = {
        minBitSet << 3,
        maxBitSet >> 4
    };
    static const std::array<uint64_t, 2> defaultRookBitboards = {
        (minBitSet << 7) | minBitSet,
        (maxBitSet >> 7) | maxBitSet
    };
    static const std::array<uint64_t, 2> defaultBishopBitboards = {
        (minBitSet << 5) | (minBitSet << 2),
        (maxBitSet >> 5) | (maxBitSet >> 2)
    };
    static const std::array<uint64_t, 2> defaultKnightBitboards = {
        (minBitSet << 6) | (minBitSet << 1),
        (maxBitSet >> 6) | (maxBitSet >> 1)
    };
    static const std::array<uint64_t, 2> defaultPawnBitboards = {
        0xFFULL << 8,
        0xFFULL << (8*6)
    };

    static const std::array<const std::array<uint64_t,2>*, 14> initMap = {
        nullptr,                       // nWhite
        nullptr,                       // nBlack
        &defaultPawnBitboards,         // wPawn
        &defaultPawnBitboards,         // bPawn
        &defaultKnightBitboards,       // wKnight
        &defaultKnightBitboards,       // bKnight
        &defaultBishopBitboards,       // wBishop
        &defaultBishopBitboards,       // bBishop
        &defaultRookBitboards,         // wRook
        &defaultRookBitboards,         // bRook
        &defaultQueenBitboards,        // wQueen
        &defaultQueenBitboards,        // bQueen
        &defaultKingBitboards,         // wKing
        &defaultKingBitboards          // bKing
    };

    for (size_t i = 0; i < bitboards.size(); ++i)
    {
        if (initMap[i] == nullptr) {
            bitboards[i] = 0;
            continue;
        }

        const bool isWhite = (i % 2 == 0);
        bitboards[i] = (*initMap[i])[isWhite ? 0 : 1];
    }

    recomputeSideOccupancies();

    // Logic Atributes Init
    sideToMove = pColor::White;
    enPassant = -1;
    castlingRights = 0b1111;
    halfMoveClock = 0;
    ply = 0;
}

// ---------------------------------
// Move make
// ---------------------------------

void Board::makeMove(Move &m)
{
    enPassant = -1;
    halfMoveClock++;
    ply++;
    
    uint64_t newPoshHash = history[ply - 1].moveHash ^ (static_cast<bool>(sideToMove) ? PieceMap::blackSideToMove : 0);
    PieceDescriptor captured = static_cast<PieceDescriptor>(0); // 0 - no capture

    const uint64_t originSq = minBitSet << m.OriginSq();
    const uint64_t targetSq = minBitSet << m.TargetSq();

    const size_t bb = getBitboard(originSq);
    const size_t bbCaptured = m.isAnyCapture() ? getBitboard(m.TargetSq()) : 0;

    const size_t WM = static_cast<bool>(sideToMove) ? 0 : 1;   // is white to move -? when yes bbIdx = bbIdx - 1

    updateOriginBirboard(originSq, targetSq, bb, newPoshHash);

    if ( m.isQuiet() )
    {
        PieceDescriptor piece = static_cast<PieceDescriptor>(bb);
        if ( piece == PieceDescriptor::bPawn || piece == PieceDescriptor::wPawn )
        {
            halfMoveClock = 0;
        }

        // Chek if en-Passant activated?
        if ( m.isDoublePawnPush() )
        {
            uint64_t mask = (targetSq << 1) | (targetSq >> 1);
            if ( bbThem(Piece::Pawn) & mask )
            {
                enPassant = m.TargetSq() + (static_cast<bool>(sideToMove) ? 8 : -8 );  
            }
        }
    }
    else if ( m.isCapture() )
    {
        halfMoveClock = 0;

        if ( m.isEpCapture() )
        {
            bitboards[bbCaptured] ^= minBitSet << ( m.TargetSq() + (static_cast<bool>(sideToMove) ? 8 : -8 ) );
        }
        else
        {
            bitboards[bbCaptured] ^= targetSq;
        }

        newPoshHash ^= PieceMap::pieceMap[bbCaptured - align][targetSq];
        captured = static_cast<PieceDescriptor>(bbCaptured);
    }
    else if ( m.isPromotion() )
    {
        newPoshHash ^= PieceMap::pieceMap[bb - align][targetSq];

        // Promotion Capture
        if ( m.isAnyCapture() )
        {
            halfMoveClock = 0;

            newPoshHash ^= PieceMap::pieceMap[bbCaptured - align][targetSq];
            bitboards[bbCaptured] ^= targetSq;
            captured = static_cast<PieceDescriptor>(bbCaptured);
        }

        // For now asume we do promotion only to Queen
        bitboards[bb] ^= targetSq;
        bitboards[static_cast<size_t>(PieceDescriptor::bQueen) - WM] ^= targetSq;
        newPoshHash ^= PieceMap::pieceMap[static_cast<size_t>(PieceDescriptor::bQueen) - WM - align][targetSq];
    }
    else if ( m.isQueenCastle() )
    {
        newPoshHash ^= PieceMap::pieceMap[static_cast<size_t>(PieceDescriptor::bRook) - WM - align][targetSq >> 2];
        setBbUs(Piece::Rook, targetSq << 3);
        newPoshHash ^= PieceMap::pieceMap[static_cast<size_t>(PieceDescriptor::bRook) - WM - align][targetSq << 3];

        castlingRights ^= static_cast<bool>(sideToMove) ? 0x01 : 0x04;
    }
    else if ( m.isKingCastle() )
    {
        newPoshHash ^= PieceMap::pieceMap[static_cast<size_t>(PieceDescriptor::bRook) - WM - align][targetSq << 1];
        setBbUs(Piece::Rook, targetSq >> 2);
        newPoshHash ^= PieceMap::pieceMap[static_cast<size_t>(PieceDescriptor::bRook) - WM - align][targetSq >> 2];
    
        castlingRights ^= static_cast<bool>(sideToMove) ? 0x02 : 0x08;
    }

    recomputeSideOccupancies();

    history[ply].moveHash      = newPoshHash;
    history[ply].capturedPiece = captured;
    history[ply].castling      = castlingRights;
    history[ply].ep            = enPassant;
    history[ply].halfmove      = halfMoveClock;
    history[ply].move          = static_cast<uint16_t>(m.getPackedMove());

    sideToMove = (sideToMove == pColor::White) ? pColor::Black : pColor::White;
}

void Board::unmakeMove()
{

}

// ---------------------------------
// Move make Helpers
// ---------------------------------

const size_t Board::getBitboard(const uint64_t sq) const
{
    for (int i = 2; i < bitboardCount; ++i)
    {
        if ( sq & bitboards[i] ) return i;
    }

    // std::unreachable();
    return 0;
}

void Board::updateOriginBirboard(const uint64_t originSq, const uint64_t targetSq, const size_t bbN, uint64_t &posHash)
{
    const uint64_t fromToBB = originSq ^ targetSq;
    
    posHash ^= PieceMap::pieceMap[bbN - align][originSq];

    bitboards[bbN] ^= fromToBB;

    posHash ^= PieceMap::pieceMap[bbN - align][std::countr_zero(targetSq)];
}

void Board::recomputeSideOccupancies() 
{
    bitboards[static_cast<size_t>(PieceDescriptor::nWhite)] =
        bitboards[static_cast<size_t>(PieceDescriptor::wPawn)]   |
        bitboards[static_cast<size_t>(PieceDescriptor::wKnight)] |
        bitboards[static_cast<size_t>(PieceDescriptor::wBishop)] |
        bitboards[static_cast<size_t>(PieceDescriptor::wRook)]   |
        bitboards[static_cast<size_t>(PieceDescriptor::wQueen)]  |
        bitboards[static_cast<size_t>(PieceDescriptor::wKing)];

    bitboards[static_cast<size_t>(PieceDescriptor::nBlack)] =
        bitboards[static_cast<size_t>(PieceDescriptor::bPawn)]   |
        bitboards[static_cast<size_t>(PieceDescriptor::bKnight)] |
        bitboards[static_cast<size_t>(PieceDescriptor::bBishop)] |
        bitboards[static_cast<size_t>(PieceDescriptor::bRook)]   |
        bitboards[static_cast<size_t>(PieceDescriptor::bQueen)]  |
        bitboards[static_cast<size_t>(PieceDescriptor::bKing)];
}
