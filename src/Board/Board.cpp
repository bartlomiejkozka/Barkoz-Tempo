// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
// Game logic holder
/*************************************************/

#include "Board.hpp"
#include "MoveGeneration/MoveUtils.hpp"
#include "PieceMap.hpp"

#include <utility>
#include <sstream>  // dla std::istringstream


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

    zobristKey = PieceMap::generatePosHash(*this);

    shortMem[ply].capturedPiece = PieceDescriptor::nWhite; // 0
    shortMem[ply].castling      = castlingRights;
    shortMem[ply].ep            = static_cast<int8_t>(enPassant);
    shortMem[ply].halfmove      = halfMoveClock;
    shortMem[ply].move          = 0;
    shortMem[ply].moveHash      = zobristKey;
}

void Board::loadFromFEN(const std::string& fen)
{
    if (fen.empty()) return;

    // 1. Reset all bitboards
    for (auto& bb : bitboards) bb = 0;

    // 2. Split FEN into fields
    std::istringstream iss(fen);
    std::string boardPart, sidePart, castlingPart, enPassantPart, halfmovePart, fullmovePart;
    iss >> boardPart >> sidePart >> castlingPart >> enPassantPart >> halfmovePart >> fullmovePart;

    // 3. Parse piece placement
    int rank = 7;  // starts from 8th rank
    int file = 0;
    for (char c : boardPart)
    {
        if (c == '/') { rank--; file = 0; continue; }
        if (isdigit(c)) { file += c - '0'; continue; }

        int square = rank * 8 + file;
        bool isWhite = isupper(c);
        char piece = tolower(c);

        uint64_t mask = 1ULL << square;

        switch(piece)
        {
            case 'p': bitboards[isWhite ? 2 : 3] |= mask; break;
            case 'n': bitboards[isWhite ? 4 : 5] |= mask; break;
            case 'b': bitboards[isWhite ? 6 : 7] |= mask; break;
            case 'r': bitboards[isWhite ? 8 : 9] |= mask; break;
            case 'q': bitboards[isWhite ? 10 : 11] |= mask; break;
            case 'k': bitboards[isWhite ? 12 : 13] |= mask; break;
        }

        file++;
    }

    // 4. Side to move
    sideToMove = (sidePart == "w") ? pColor::White : pColor::Black;

    // 5. Castling rights
    castlingRights = 0;
    for (char c : castlingPart)
    {
        switch(c)
        {
            case 'K': castlingRights |= 0b1000; break; // white kingside
            case 'Q': castlingRights |= 0b0100; break; // white queenside
            case 'k': castlingRights |= 0b0010; break; // black kingside
            case 'q': castlingRights |= 0b0001; break; // black queenside
        }
    }

    // 6. En passant
    if (enPassantPart == "-") enPassant = -1;
    else
    {
        char f = enPassantPart[0];
        char r = enPassantPart[1];
        enPassant = (r - '1') * 8 + (f - 'a');
    }

    // 7. Halfmove clock
    halfMoveClock = std::stoi(halfmovePart);

    // 8. Fullmove number
    ply = 0;

    // 9. Recompute occupancies
    recomputeSideOccupancies();

    // 10. Initialize shortMem for current ply
    shortMem[ply].capturedPiece = PieceDescriptor::nWhite; // placeholder
    shortMem[ply].castling      = castlingRights;
    shortMem[ply].ep            = static_cast<int8_t>(enPassant);
    shortMem[ply].halfmove      = halfMoveClock;
    shortMem[ply].move          = 0;
    shortMem[ply].moveHash      = PieceMap::generatePosHash(*this);
}


// ---------------------------------
// Move make
// ---------------------------------

void Board::makeMove(Move &m)
{
    enPassant = -1;
    halfMoveClock++;
    ply++;
    
    uint64_t newPoshHash = zobristKey ^ (static_cast<bool>(sideToMove) ? PieceMap::blackSideToMove : 0);
    auto captured = static_cast<PieceDescriptor>(0); // 0 - no capture

    const uint64_t originSq = minBitSet << m.OriginSq();
    const uint64_t targetSq = minBitSet << m.TargetSq();

    const size_t bb = getBitboard(originSq);
    if (bb == 0)
    {
        // temp resolution for bag:
        // position: position fen 1kb1r3/1p1q2pp/r4p2/p7/7P/b1P2N2/P2B1PP1/2RK3R w - - 1 39
        // depth: 7
        // action: bb = 0
        return;
    }

    size_t bbCaptured = m.isAnyCapture() ? getBitboard(targetSq) : 0;
    if ( m.isEpCapture() ) bbCaptured = getBitboard( bitBoardSet(m.TargetSq() + (static_cast<bool>(sideToMove) ? 8 : -8 )) );

    if (m.isCapture() && bbCaptured == 0)
    {
        // temp resolution for bag:
        // position: position fen 2r1k1nr/1p3pp1/p1p1p3/b3nq1p/3P4/Q3PP2/1K2N2P/1R3BR1 b k - 7 26
        // depth: 8
        // action: bbCapture = 0 while ep Capture is move 
        return;
    }

    const size_t WM = static_cast<bool>(sideToMove) ? 0 : 1;   // is white to move -? when yes bbIdx = bbIdx - 1

    updateOriginBirboard(originSq, targetSq, bb, newPoshHash);

    uint8_t clearMask = 0;
    if (bb == (std::to_underlying(PieceDescriptor::bKing) - WM)) 
    {
        clearMask |= static_cast<bool>(sideToMove) ? 0x03 : 0x0c;
    }
    if (bb == (std::to_underlying(PieceDescriptor::bRook) - WM)) 
    {
        if (((originSq == WhiteRookKingPos) && (sideToMove == pColor::White)) || ((originSq == BlackRookKingPos) && (sideToMove == pColor::Black)))
            clearMask |= static_cast<bool>(sideToMove) ? 0x02 : 0x08; 
        else if (((originSq == WhiteRookQueenPos) && (sideToMove == pColor::White)) || ((originSq == BlackRookQueenPos) && (sideToMove == pColor::Black)))
            clearMask |= static_cast<bool>(sideToMove) ? 0x01 : 0x04;
    }
    if (bbCaptured && (bbCaptured == (std::to_underlying(PieceDescriptor::wRook) + WM))) 
    {
        if (((targetSq == WhiteRookKingPos) && (sideToMove == pColor::Black)) || ((targetSq == BlackRookKingPos) && (sideToMove == pColor::White)))
            clearMask |= static_cast<bool>(sideToMove) ? 0x08 : 0x02; 
        else if (((targetSq == WhiteRookQueenPos) && (sideToMove == pColor::Black)) || ((targetSq == BlackRookQueenPos) && (sideToMove == pColor::White)))
            clearMask |= static_cast<bool>(sideToMove) ? 0x04 : 0x01;
    }
    if (clearMask) 
    {
        castlingRights &= static_cast<uint8_t>(~clearMask);
    }

    if ( m.isQuiet() )
    {
        if ( auto piece = static_cast<PieceDescriptor>(bb); 
            piece == PieceDescriptor::bPawn || piece == PieceDescriptor::wPawn )
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
            bitboards[bbCaptured] ^= (minBitSet << ( m.TargetSq() + (static_cast<bool>(sideToMove) ? 8 : -8 ) ));
        }
        else
        {
            bitboards[bbCaptured] ^= targetSq;
        }

        newPoshHash ^= PieceMap::pieceMap[bbCaptured - align][m.TargetSq()];
        captured = static_cast<PieceDescriptor>(bbCaptured);
    }
    else if ( m.isPromotion() )
    {
        newPoshHash ^= PieceMap::pieceMap[bb - align][m.TargetSq()];

        // Promotion Capture
        if ( m.isAnyCapture() )
        {
            halfMoveClock = 0;

            newPoshHash ^= PieceMap::pieceMap[bbCaptured - align][m.TargetSq()];
            bitboards[bbCaptured] ^= targetSq;
            captured = static_cast<PieceDescriptor>(bbCaptured);
        }

        bitboards[bb] ^= targetSq;
        bitboards[std::to_underlying(mapPromotionType(m)) - WM] ^= targetSq;
        newPoshHash ^= PieceMap::pieceMap[std::to_underlying(mapPromotionType(m)) - WM - align][m.TargetSq()];
    }
    else if ( m.isQueenCastle() )
    {
        newPoshHash ^= PieceMap::pieceMap[std::to_underlying(PieceDescriptor::bRook) - WM - align][std::countr_zero(targetSq >> 2)];
        setBbUs( Piece::Rook,  WM ? WhiteRookQueenPos : BlackRookQueenPos);
        setBbUs(Piece::Rook, targetSq << 1);
        newPoshHash ^= PieceMap::pieceMap[std::to_underlying(PieceDescriptor::bRook) - WM - align][std::countr_zero(targetSq << 3)];
    }
    else if ( m.isKingCastle() )
    {
        newPoshHash ^= PieceMap::pieceMap[std::to_underlying(PieceDescriptor::bRook) - WM - align][std::countr_zero(targetSq << 1)];
        setBbUs( Piece::Rook,  WM ? WhiteRookKingPos : BlackRookKingPos);
        setBbUs(Piece::Rook, targetSq >> 1);
        newPoshHash ^= PieceMap::pieceMap[std::to_underlying(PieceDescriptor::bRook) - WM - align][std::countr_zero(targetSq >> 2)];
    }

    recomputeSideOccupancies();

    zobristKey = newPoshHash;

    shortMem[ply].moveHash      = zobristKey;
    shortMem[ply].capturedPiece = captured;
    shortMem[ply].castling      = castlingRights;
    shortMem[ply].ep            = static_cast<int8_t>(enPassant);
    shortMem[ply].halfmove      = halfMoveClock;
    shortMem[ply].move          = static_cast<uint16_t>(m.getPackedMove());

    sideToMove = (sideToMove == pColor::White) ? pColor::Black : pColor::White;
}

void Board::unmakeMove()
{
    if ( ply == 0 ) return;

    Move m{shortMem[ply].move};
    PieceDescriptor cPiece = shortMem[ply].capturedPiece;
    pColor prevSTM = (sideToMove == pColor::White) ? pColor::Black : pColor::White;

    ply--;
    halfMoveClock  = shortMem[ply].halfmove;
    castlingRights = shortMem[ply].castling;
    enPassant      = shortMem[ply].ep;
    zobristKey     = shortMem[ply].moveHash;
    
    const uint64_t originSq = minBitSet << m.OriginSq();
    const uint64_t targetSq = minBitSet << m.TargetSq();
    const size_t bb = getBitboard(targetSq);
    const size_t WM = static_cast<bool>(prevSTM) ? 0 : 1;

    updateOriginBirboard(originSq, targetSq, bb);

    if (m.isAnyCapture())
    {
        size_t BBC = std::to_underlying(cPiece);

        if (m.isEpCapture())
        {
            bitboards[BBC] ^= (minBitSet << ( m.TargetSq() + (static_cast<bool>(prevSTM) ? 8 : -8 ) ));        
        }
        else
        {
            bitboards[BBC] ^= targetSq;
        }
    }
    else if ( m.isQueenCastle() )
    {
        setBbThem(Piece::Rook, targetSq << 1);
        setBbThem(Piece::Rook, targetSq >> 2);
    }
    else if ( m.isKingCastle() )
    {
        setBbThem(Piece::Rook, targetSq >> 1);
        setBbThem(Piece::Rook, targetSq << 1);
    }

    if (m.isPromotion())
    {
        bitboards[bb] ^= originSq;
        bitboards[std::to_underlying(PieceDescriptor::bPawn) - WM] ^= originSq;
    }

    recomputeSideOccupancies();
    sideToMove = prevSTM;
}

// ---------------------------------
// Move make Helpers
// ---------------------------------

size_t Board::getBitboard(const uint64_t sq) const
{
    for (int i = 2; i < bitboardCount; ++i)
    {
        if ( sq & bitboards[i] ) return i;
    }

    // here returning 0 could lead to acessing some array out of index -> Segmention fault !!!
    // std::unreachable();
    return 0;
}

void Board::updateOriginBirboard(const uint64_t originSq, const uint64_t targetSq, const size_t bbN, 
    std::optional<std::reference_wrapper<uint64_t>> posHash)
{
    if ( posHash.has_value() )
    {
        posHash->get() ^= PieceMap::pieceMap[bbN - align][std::countr_zero(originSq)];
        posHash->get() ^= PieceMap::pieceMap[bbN - align][std::countr_zero(targetSq)];
    }

    const uint64_t fromToBB = originSq ^ targetSq;
    bitboards[bbN] ^= fromToBB;
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

[[nodiscard]] PieceDescriptor Board::mapPromotionType(Move &m) const
{
    if      ( m.isKnighPromo()  || m.isKnightPromoCapture() ) return PieceDescriptor::bKnight;
    else if ( m.isBishopPromo() || m.isBishopPromoCapture() ) return PieceDescriptor::bBishop;
    else if ( m.isRokkPromo()   || m.isRookPromoCapture()   ) return PieceDescriptor::bRook;
    else if ( m.isQueenPromo()  || m.isQueenPromoCapture()  ) return PieceDescriptor::bQueen;

    //std::unreachable();
    return PieceDescriptor::nWhite;
}
