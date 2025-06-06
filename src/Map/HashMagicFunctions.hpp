#ifndef HASH_MAGIC_FUNCTIONS_HPP
#define HASH_MAGIC_FUNCTIONS_HPP

#include <cstdint>
#include <chrono>
#include <random>
#include <utility>


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
    
    // Main usage to find the magic
    explicit FancyMagicFunction(const uint8_t offset) {  }
    
    constexpr FancyMagicFunction(const FancyPair &params):
        : _magic(magic), _offset(offset) {}

    // Probably unused
    constexpr FancyMagicFunction(const uint64_t magic, const uint8_t offset)
        : _magic(magic), _offset(offset) {}





    private:
    uint64_t _magic;
    uint8_t _offset;
};


#endif
