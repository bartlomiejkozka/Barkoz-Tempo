#ifndef STATE_INFO_HPP
#define STATE_INFO_HPP

#include <cstdint>

// maybe type of el. in std::stack
struct StateInfo
{
    // stockFish also use pawnKey hash, materialKey hash pliesFromNull
    uint64_t hash;
    uint64_t from;
    uint64_t to;
    uint8_t castlingRights;
    uint8_t enPassant;
    uint8_t halfMoveClock;
    uint8_t capturedPiece;
};


#endif