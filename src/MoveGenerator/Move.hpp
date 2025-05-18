#ifndef MOVE_HPP
#define MOVE_HPP

#include <cstdint>

#include "Board.hpp"

// TODO:
// - class Move for packing packed_move and have the move method and unmove with Bitboards change
// - optional add some special bits fileds to packed move to have 32 bits instead of 16

// - add inBetween 2d array (generator)

// - on the fly move logic
// - add simple evaluation with legal moves checks



/*
Packed move bits meaning:
    0-5:    from square
    6-11:   to square
    12:     promotion
    13:     capture
    14:     special 1
    15:     special 0

Encoding:
    0:  0 0 0 0 - quiet move (no capure, no promotion)
    1:  0 0 0 1 - double pawn push
    2:  0 1 0 - king castle
    3:  0 1 1 - queen castle
    4:  0 1 0 0 - caputres
    5:  0 1 0 1 - ep-caputre
    8:  1 0 0 0 - knigh-promotion
    9:  1 0 0 1 - bishop-promotion
    10:  1 0 1 0 - rook-promotion
    11: 1 0 1 1 - queen-promotion
    12: 1 1 0 0 - knight-promo capture
    13: 1 1 0 1 - bishop-promo capture
    14: 1 1 1 0 - rook-promo capture
    15: 1 1 1 1 - queen-promo capture

* For now every move type will have encoded value form 1 - 15 in decimal to qucik checks the type of move
* This causes losses on felxibility - be aware
*/
struct packedMove
{
    private:
    uint16_t _packed_move;

    public:

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

    // unused for this logic implementation
    
    /*
    [[nodiscard]] constexpr bool isPromotion() { return (_packed_move & promotion_mask) != 0; }

    [[nodiscard]] constexpr bool isCapture() { return (_packed_move & capture_mask) != 0; }

    [[nodiscard]] constexpr bool isSpecial1() { return (_packed_move & special_1_mask) != 0; }

    [[nodiscard]] constexpr bool isSpecial0() { return (_packed_move & special_0_mask) != 0; }
    */
};


class Move 
{
    private:
    packedMove packed_move;

    public:

    //-------HELPERS--------

    //--------------------
    // Typed of move
    //--------------------

    [[nodiscard]] constexpr bool isQuiet() { return packed_move.getSpecials() == 0; }

    [[nodiscardd]] constexpr bool isDoublePawnPush() { return packed_move.getSpecials() == 1; }

    [[nodiscardd]] constexpr bool isKingCastle() { return packed_move.getSpecials() == 2; }

    [[nodiscardd]] constexpr bool isQueenCastle() { return packed_move.getSpecials() == 3; }

    [[nodiscardd]] constexpr bool isJustCapture() { return packed_move.getSpecials() == 4; }

    [[nodiscardd]] constexpr bool isEpCapture() { return packed_move.getSpecials() == 5; }

    [[nodiscardd]] constexpr bool isKnighPromo() { return packed_move.getSpecials() == 8; }

    [[nodiscardd]] constexpr bool isBishopPromo() { return packed_move.getSpecials() == 9; }

    [[nodiscardd]] constexpr bool isRokkPromo() { return packed_move.getSpecials() == 10; }

    [[nodiscardd]] constexpr bool isQueenPromo() { return packed_move.getSpecials() == 11; }

    [[nodiscardd]] constexpr bool isKnightPromoCapture() { return packed_move.getSpecials() == 12; }

    [[nodiscardd]] constexpr bool isBishopPromoCapture() { return packed_move.getSpecials() == 13; }

    [[nodiscardd]] constexpr bool isRookPromoCapture() { return packed_move.getSpecials() == 14; }

    [[nodiscardd]] constexpr bool isQueenPromoCapture() { return packed_move.getSpecials() == 15; }

    //--------METHODS------

    void make_move(Board &b)
    {
        
    }

    void unmake_move(Board &b)
    {

    }
};

#endif
