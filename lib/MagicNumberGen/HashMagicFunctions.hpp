#ifndef HASH_MAGIC_FUNCTIONS_HPP
#define HASH_MAGIC_FUNCTIONS_HPP

#include <cstdint>
#include <chrono>
#include <random>
#include <utility>

// TODO:
//      Here, global defintion of attackTab, then in Magic Pieces pointer to first attackTab bitboard.

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

    private:

    // in case failure (max interation exceed) should throw/return
    constexpr uint64_t generateMagic(const uint8_t offset, const uint64_t occupancies, const uint64_t square)
    {
        // generator init
        RandomGenerator rng;
        std::random_device rd;
        rng.seed(rd());

        uint64_t magic = rng();
        
    }

    uint64_t _magic;
    uint8_t _offset;
};


#endif
