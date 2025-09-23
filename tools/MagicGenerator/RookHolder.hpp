#ifndef ROOK_HOLDER
#define ROOK_HOLDER

#include "MagicHolder.hpp"

struct RookHolder final : MagicHolder
{
    uint64_t occupanciesMask(const int square) const noexcept override
    {
        return (MoveUtils::inBetween[square][square + (7 - (square % 8))]        // right
                | MoveUtils::inBetween[square][square - (square % 8)]            // left       
                | MoveUtils::inBetween[square][square + 8 * (7 - (square / 8))]  // up
                | MoveUtils::inBetween[square][square - 8 * (square / 8)]);      // down
    }

    /* chessprogramming */
    uint64_t attacksMask(const int square, const uint64_t block) const noexcept override
    {
        uint64_t result = 0ULL;
        int rk = square/8;
		int fl = square%8;
		int r, f;
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
};

#endif
