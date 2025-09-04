#ifndef HASH_MAGIC_FUNCTIONS_HPP
#define HASH_MAGIC_FUNCTIONS_HPP

#include <cstdint>
#include <chrono>
#include <random>
#include <utility>

#include "MoveUtils.hpp"

// TODO:
//        
// - Probably in another TU, global defintion of attackTab, then in Magic Pieces pointer to first attackTab bitboard.

/*
* The Class concept is dedicated to
* generate random unique index for magic boards by brude force method - "Trial and Error"
* hold magic number and offset for specific bishop/rook position
*/

typename<class RandomGenerator = std::mt19937_64>
class FancyMagicFunction {

    // [magic number, offset]
    using FancyPair std::pair<uint64_t, uint8_t>;

    public:
    
    //--------------------
    // Contants
    //--------------------
    static constexpr int maxIndex = 4096;   // maxium combination number of occupancies per square

    //--------------------
    // Intilizators
    //--------------------
    
    explicit FancyMagicFunction(const uint8_t offset, const uint64_t occupancies, const uint64_t square)
        : _magic(generateMagic(offset, occupancies, square)), _offset(offset) {}
    
    constexpr FancyMagicFunction(const FancyPair &params):
        : _magic(magic), _offset(offset) {} 

    constexpr FancyMagicFunction(const uint64_t magic, const uint8_t offset)
        : _magic(magic), _offset(offset) {}

    //--------------------
    // Getters
    //--------------------

    constexpr uint16_t getMagic()    { return _magic; }
    constexpr uint8_t  getOffset()   { return _offset; }

    //--------------------

    constexpr uint64_t rmask(const int square)
    {
        return (MoveUtils.inBetween[square][square + (7 - (square % 8))]        // right
                | MoveUtils.inBetween[square][square - (square % 8)]            // left       
                | MoveUtils.inBetween[square][square + 8 * (7 - (square / 8))]  // up
                | MoveUtils.inBetween[square][square - 8 * (square / 8)]);      // down
    }

    constexpr uint64_t bmask_bishop(const int square)
    {    
        return (MoveUtils.inBetween[square][square + 9 * std::min(7 - (square % 8), 7 - (square / 8))]) // up-right
             | (MoveUtils.inBetween[square][square + 7 * std::min((square % 8), 7 - (square / 8))])     // up-left
             | (MoveUtils.inBetween[square][square - 7 * std::min(7 - (square % 8), (square / 8))])     // down-right
             | (MoveUtils.inBetween[square][square - 9 * std::min((square % 8), (square / 8))]);        // down-left
    }


    private:

    uint64_t _magic;
    uint8_t _offset;
};


#endif
