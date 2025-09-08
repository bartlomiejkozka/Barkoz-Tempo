#ifndef BISHOP_HOLDER
#define BISHOP_HOLDER

#include "MagicHolder.hpp"

struct BishopHolder final : MagicHolder
{
    uint64_t occupanciesMask(const int square) const noexcept override
    {
        return (MoveUtils::inBetween[square][square + 9 * std::min(7 - (square % 8), 7 - (square / 8))]  // up-right
             |  MoveUtils::inBetween[square][square + 7 * std::min((square % 8), 7 - (square / 8))]      // up-left
             |  MoveUtils::inBetween[square][square - 7 * std::min(7 - (square % 8), (square / 8))]      // down-right
             |  MoveUtils::inBetween[square][square - 9 * std::min((square % 8), (square / 8))]);        // down-left
    }

    /* chessprogramming */
    uint64_t attacksMask(const int square, const uint64_t block) const noexcept override
    {
        uint64_t result = 0ULL;
        int rk = square/8;
		int fl = square%8;
		int r, f;
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
};

#endif
