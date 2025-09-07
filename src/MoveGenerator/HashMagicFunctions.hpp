#ifndef HASH_MAGIC_FUNCTIONS_HPP
#define HASH_MAGIC_FUNCTIONS_HPP

#include <cstdint>
#include <chrono>
#include <random>
#include <utility>
#include <bit>

#include "MoveUtils.hpp"
#include "BitOperation.hpp"


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
        return (MoveUtils.inBetween[square][square + 9 * std::min(7 - (square % 8), 7 - (square / 8))]  // up-right
             |  MoveUtils.inBetween[square][square + 7 * std::min((square % 8), 7 - (square / 8))]      // up-left
             |  MoveUtils.inBetween[square][square - 7 * std::min(7 - (square % 8), (square / 8))]      // down-right
             |  MoveUtils.inBetween[square][square - 9 * std::min((square % 8), (square / 8))]);        // down-left
    }

    // computes one of occupanices combination based on occupancie index - max combinations=4096
    // * param[in] : bits -> occupaniec number - max=12
    // * param[in] : mask -> occupancies mask
    /* chessprogramming */
    constexpr uint64_t index_to_uint64(const int index, const int bits, const uint64_t mask) 
    {
        uint64_t result = 0ULL;
        for(int i = 0; i < bits; i++)
        {
            int j = pop_1st(&mask);
            if (index & (1 << i))
            {
                result |= (1ULL << j);
            }
        }
        return result;
    }

    /* chessprogramming */
    constexpr uint64_t ratt(const int sq, const uint64_t block)
    {
        uint64_t result = 0ULL;
        int rk = sq/8, fl = sq%8, r, f;
        for(r = rk+1; r <= 7; r++) 
        {
          result |= (1ULL << (fl + r*8));
          if(block & (1ULL << (fl + r*8))) break;
        }
        for(r = rk-1; r >= 0; r--) 
        {
          result |= (1ULL << (fl + r*8));
          if(block & (1ULL << (fl + r*8))) break;
        }
        for(f = fl+1; f <= 7; f++) 
        {
          result |= (1ULL << (f + rk*8));
          if(block & (1ULL << (f + rk*8))) break;
        }
        for(f = fl-1; f >= 0; f--) 
        {
          result |= (1ULL << (f + rk*8));
          if(block & (1ULL << (f + rk*8))) break;
        }
        return result;
      }
      
    /* chessprogramming */
    uint64_t batt(int sq, uint64_t block) 
    {
        uint64_t result = 0ULL;
        int rk = sq/8, fl = sq%8, r, f;
        for(r = rk+1, f = fl+1; r <= 7 && f <= 7; r++, f++) 
        {
          result |= (1ULL << (f + r*8));
          if(block & (1ULL << (f + r * 8))) break;
        }
        for(r = rk+1, f = fl-1; r <= 7 && f >= 0; r++, f--) 
        {
          result |= (1ULL << (f + r*8));
          if(block & (1ULL << (f + r * 8))) break;
        }
        for(r = rk-1, f = fl+1; r >= 0 && f <= 7; r--, f++) 
        {
          result |= (1ULL << (f + r*8));
          if(block & (1ULL << (f + r * 8))) break;
        }
        for(r = rk-1, f = fl-1; r >= 0 && f >= 0; r--, f--) 
        {
          result |= (1ULL << (f + r*8));
          if(block & (1ULL << (f + r * 8))) break;
        }
        return result;
    }



    private:

    uint64_t _magic;
    uint8_t _offset;
};


#endif
