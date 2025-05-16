#ifndef STATE_INFO_HPP
#define STATE_INFO_HPP

#include <cstdint>
#include <array>
#include <stdexcept>


struct StateInfo_t {
    // stockFish also use pawnKey hash, materialKey hash pliesFromNull
    uint64_t hash;
    uint64_t from;
    uint64_t to;
    uint8_t castlingRights;
    uint8_t enPassant;
    uint8_t halfMoveClock;
    uint8_t capturedPiece;
};

/*
The postion info is stored in fixed size array of depth search.
Having pointers to prev el. like in backword list, which is stored in array to avoid dynamic allocation.
*/
class Position
{
private:
    std::array<StateInfo_t, 256> PositionStack; // assumes for now depth sieze = 256

public:
    //--------Contructors-----------
    Position() = default;
    ~Position() = default;
    Position(Position &other) = default;
    Position& operator=(const Position& other) = default;

    //---------Methods---------------
    /*
    Puhs method which have to get move type var and opptional Board attribs
    */

};

#endif