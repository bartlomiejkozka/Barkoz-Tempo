#ifndef BITOPERATION_HPP
#define BITOPERATION_HPP

#include <cstdint>
#include <bit>

// TODO
//
// - Consider snake case name convenction for this module -> in future could be consider as minor lib

//BitOperation constants
constexpr uint64_t minBitSet = static_cast<uint64_t>(1);
constexpr uint64_t maxBitSet = static_cast<uint64_t>(1) << 63;

//BitOperation functions
inline uint64_t convertFromMinBitSetToMaxBitSet(uint64_t bitSet)
{
    return bitSet ^ 63;
}

inline int pop_1st(uint64_t &mask)
{
    int idx = std::countr_zero(mask);
    mask &= (mask - 1);
    return idx;
}

inline int count_1s(uint64_t mask)
{
    int i;
    for (i = 0; mask; ++i, mask &= mask - 1) {}
    return i;
}

[[nodiscard]] inline constexpr uint64_t bitBoardSet(int sq)
{
    return minBitSet << sq;
}

#endif // BITOPERATION_HPP
