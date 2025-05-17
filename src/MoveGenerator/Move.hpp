#ifndef MOVE_HPP
#define MOVE_HPP

#include <cstdint>

#include "Board.hpp"

// TODO:
// - method in packed_move for getting speciffic bits
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
    0 0 0 0 - quiet move (no capure, no promotion)
    0 0 0 1 - double pawn push
    0 0 1 0 - king castle
    0 0 1 1 - queen castle
    0 1 0 0 - caputres
    0 1 0 1 - ep-caputre
    1 0 0 0 - knigh-promotion
    1 0 0 1 - bishop-promotion
    1 0 1 0 - rook-promotion
    1 0 1 1 - queen-promotion
    1 1 0 0 - knight-promo capture
    1 1 0 1 - bishop-promo capture
    1 1 1 0 - rook-promo capture
    1 1 1 1 - queen-promo capture

*/
struct packed_move
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

};


class Move 
{

};

#endif
