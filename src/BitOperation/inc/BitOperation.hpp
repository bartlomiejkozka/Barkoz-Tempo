#ifndef BITOPERATION_HPP
#define BITOPERATION_HPP

#include <cstdint>
#include <bit>


//BitOperation constants
constexpr uint64_t minBitSet = static_cast<uint64_t>(1);
constexpr uint64_t maxBitSet = static_cast<uint64_t>(1) << 63;

//BitOperation functions
inline uint64_t convertFromMinBitSetToMaxBitSet(uint64_t bitSet)
{
    return bitSet ^ 63;
}

inline int pop_1st(uint64_t *mask)
{
    int idx = countr_zero(*mask);
    *mask &= (*mask - 1);
    return idx;
}

#endif // BITOPERATION_HPP