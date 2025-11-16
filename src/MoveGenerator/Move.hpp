#ifndef MOVE_HPP
#define MOVE_HPP

#include <cstdint>

#include "Board.hpp"

// TODO:
// - class Move for packing packed_move and have the move method and unmove with Bitboards change
// - on the fly move logic
// - add simple evaluation with legal moves checks


/*
Packed move bits meaning:
    0-5:    origin square
    6-11:   target square
    12:     promotion (promoted piece encoded on special bits)
    13:     capture
    14:     special 1
    15:     special 0

Last 4 Nibble bits Encoding:
    0:  0 0 0 0 - quiet move (no capure, no promotion)
    1:  0 0 0 1 - double pawn push
    2:  0 0 1 0 - king castle
    3:  0 0 1 1 - queen castle
    4:  0 1 0 0 - caputres
    5:  0 1 0 1 - ep-caputre
    8:  1 0 0 0 - knigh-promotion
    9:  1 0 0 1 - bishop-promotion
    10: 1 0 1 0 - rook-promotion
    11: 1 0 1 1 - queen-promotion
    12: 1 1 0 0 - knight-promo capture
    13: 1 1 0 1 - bishop-promo capture
    14: 1 1 1 0 - rook-promo capture
    15: 1 1 1 1 - queen-promo capture

* This implementation encode value form 1 - 15 in decimal to qucik checks the type of move
* It is possible to store moves on 32 bits (with capture, ...), but it's much worse for Transposition table memory
*/
struct packedMove
{
private:
    uint16_t _packed_move;

public:
    //---------INITIALIZER--------

    explicit packedMove(uint16 packedMove) : _packed_move(packedMove);

    //---------BIT MASKS--------

    static constexpr uint16_t originBits_mask =  0b1111110000000000;
    static constexpr uint16_t targetBits_mask =  0b0000001111110000;
    static constexpr uint16_t specialBits_mask = 0xF;
    static constexpr uint16_t promotion_mask =   0b0000000000001000;
    static constexpr uint16_t capture_mask =     0b0000000000000100;
    static constexpr uint16_t special_1_mask =   0b0000000000000010;
    static constexpr uint16_t special_0_mask =   0b0000000000000001;

    //--------METHODS---------

    [[nodiscard]] constexpr uint16_t getOrigin() { return (_packed_move & originBits_mask) >> 10; }

    [[nodiscard]] constexpr uint16_t getTarget() { return (_packed_move & targetBits_mask) >> 4; }

    [[nodiscard]] constexpr uint16_t getSpecials() { return _packed_move & specialBits_mask; }
};


// structure to save irrevesible attributes + packed move, necessary to unmake move
struct Undo
{
    uint8_t castling;              // 0b00001(white kingside)1(white queenside)1(black kingside)1(balck queenside)
    int8_t ep;                     // enPassant Square, -1 - if no enPassant
    uint8_t halfmove;
    PieceDescriptor capturedPiece; // type+color of captured piece, 0 if none
    uint16_t move;                 // packedMove
    uint64_t moveHash;
};


enum class MoveType : uint16_t 
{
    QUIET        = 0, 
    DOUBLE_PUSH  = 1, 
    KING_CASTLE  = 2, 
    QUEEN_CASTLE = 3,
    CAPTURE      = 4, 
    EP_CAPTURE   = 5,
    KN_PROM      = 8, 
    B_PROM       = 9, 
    R_PROM       = 10, 
    Q_PROM       = 11,
    KN_PROM_CAP  = 12, 
    B_PROM_CAP   = 13, 
    R_PROM_CAP   = 14,
    Q_PROM_CAP   = 15
};


class Move 
{
private:
    packedMove packed_move;

public:
    static constexpr int OriginSq = 12;
    static constexpr int TargetSq = 6;

    //------------------------------------
    // Initializers
    //------------------------------------

    explicit Move(int origin, int target, MoveType type)
        : packedMove(static_cast<uint16_t>( (type << OriginSqOriginSq) | (target << TargetSq) | type )); 

    explicit Move(int origin, int target, uint16_t type)
        : packedMove(static_cast<uint16_t>( (type << OriginSqOriginSq) | (target << TargetSq) | type ));
    
    //------------------------------------
    // Typed of move
    //------------------------------------

    //--------------------
    // Quiets
    //--------------------

    [[nodiscard]] constexpr bool isClearQuiet() { return packed_move.getSpecials() == 0; }

    [[nodiscard]] constexpr bool isDoublePawnPush() { return packed_move.getSpecials() == 1; }

    [[nodiscard]] constexpr bool isQuiet() { return isClearQuiet() | isDoublePawnPush(); }

    //--------------------
    // Castles
    //--------------------

    [[nodiscard]] constexpr bool isKingCastle() { return packed_move.getSpecials() == 2; }

    [[nodiscard]] constexpr bool isQueenCastle() { return packed_move.getSpecials() == 3; }

    //--------------------
    // Captures
    //--------------------

    // without promo captures - only clear captures & en-passant captures
    [[nodiscard]] constexpr bool isCapture() { return (packed_move.getSpecials() & 0b1100) == 4; }

    [[nodiscard]] constexpr bool isJustCapture() { return packed_move.getSpecials() == 4; }

    [[nodiscard]] constexpr bool isEpCapture() { return packed_move.getSpecials() == 5; }

    //--------------------
    // Promotions
    //--------------------

    // already with check is Promotion-Capture
    [[nodiscard]] constexpr bool isPromotion() { return (packed_move.getSpecials() & 0b1000) == 8; }

    [[nodiscard]] constexpr bool isKnighPromo() { return packed_move.getSpecials() == 8; }

    [[nodiscard]] constexpr bool isBishopPromo() { return packed_move.getSpecials() == 9; }

    [[nodiscard]] constexpr bool isRokkPromo() { return packed_move.getSpecials() == 10; }

    [[nodiscard]] constexpr bool isQueenPromo() { return packed_move.getSpecials() == 11; }


    //--------------------
    // Promotion-Captures
    //--------------------
     
    [[nodiscard]] constexpr bool isAnyCapture() { return packed_move.getSpecials() & 0b0100; }

    [[nodiscard]] constexpr bool isKnightPromoCapture() { return packed_move.getSpecials() == 12; }

    [[nodiscard]] constexpr bool isBishopPromoCapture() { return packed_move.getSpecials() == 13; }

    [[nodiscard]] constexpr bool isRookPromoCapture() { return packed_move.getSpecials() == 14; }

    [[nodiscard]] constexpr bool isQueenPromoCapture() { return packed_move.getSpecials() == 15; }

    //------------------------------------
    // Getters
    //------------------------------------

    static constexpr uint16_t OriginSqFlag = 0x3F;

    int OriginSq() { return packed_move & OriginSqFlag; }
    int TargetSq() { return (packed_move >> 6) & OriginSqFlag; }
};

#endif
