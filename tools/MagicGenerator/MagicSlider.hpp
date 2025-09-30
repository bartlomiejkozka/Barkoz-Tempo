#ifndef MAGIC_HOLDER
#define MAGIC_HOLDER

#include <cstdint>

// TODO: Change this virtualization to templates and specialization

enum class Slider
{
    Rook,
    Bishop
};

template<Slider> struct MagicSlider;

/*---------SPECIALIZATIONS---------*/

template<> struct MagicSlider<Slider::Rook>
{
    [[nodiscard("PURE FUN")]] static constexpr uint64_t occupanciesMask(const int square)
    {
        return (MoveUtils::inBetween[square][square + (7 - (square % 8))]        // right
                | MoveUtils::inBetween[square][square - (square % 8)]            // left       
                | MoveUtils::inBetween[square][square + 8 * (7 - (square / 8))]  // up
                | MoveUtils::inBetween[square][square - 8 * (square / 8)]);      // down
    }

    /* chessprogramming */
    [[nodiscard("PURE FUN")]] static uint64_t attacksMask(const int square, const uint64_t block)
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

template<> struct MagicSlider<Slider::Bishop>
{
    [[nodiscard("PURE FUN")]] static constexpr uint64_t occupanciesMask(const int square)
    {
        return (MoveUtils::inBetween[square][square + 9 * std::min(7 - (square % 8), 7 - (square / 8))]  // up-right
             |  MoveUtils::inBetween[square][square + 7 * std::min((square % 8), 7 - (square / 8))]      // up-left
             |  MoveUtils::inBetween[square][square - 7 * std::min(7 - (square % 8), (square / 8))]      // down-right
             |  MoveUtils::inBetween[square][square - 9 * std::min((square % 8), (square / 8))]);        // down-left
    }

    /* chessprogramming */
    [[nodiscard("PURE FUN")]] static uint64_t attacksMask(const int square, const uint64_t block)
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
