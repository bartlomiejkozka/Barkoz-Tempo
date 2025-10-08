#ifndef MAGIC_HOLDER
#define MAGIC_HOLDER

#include <cstdint>
#include <array>
#include <cstdint>

#define BOARD_SIZE  (64)

// TODO: Change this virtualization to templates and specialization

enum class Slider
{
    Rook,
    Bishop
};

/*
    2D Array containing hard-coded bitbards with bits set that are between (vertical, horizoontal, diagonal) from-to indicies
    * For the not hirozontal, vertical or diagonal squares array value is set to zeros bit board
*/
inline const std::array<std::array<uint64_t,BOARD_SIZE>, BOARD_SIZE> inBetween = [] () {
    std::array<std::array<uint64_t, BOARD_SIZE>, BOARD_SIZE> tab = {};

    for (size_t i = 0; i < BOARD_SIZE; ++i)
    {
        for (size_t j = 0; j < BOARD_SIZE; ++j)
        {
            if (i == j)
            {
                continue;
            }
            int min = (i < j ? i : j);
            int max = (i > j ? i : j);
            if (i / 8 == j / 8)
            {
                tab[i][j] = static_cast<uint64_t>((1ULL << (std::abs(static_cast<int>(i) - static_cast<int>(j)) - 1)) - 1) << (min + 1);
            }
            else if (i % 8 == j % 8)
            {
                for (int it = min+8; it < max; it+=8) 
                {
                    tab[i][j] |= static_cast<uint64_t>(1) << it;
                }
            }
            else if (std::abs(static_cast<int>(i)/8 - static_cast<int>(j)/8) == std::abs(static_cast<int>(i)%8 - static_cast<int>(j)%8))
            {
                int diag = 1;
                if (min % 8 > max % 8)
                {
                    diag = -1;
                }
                for (int it = min+8+diag; it < max; it+=8+diag) 
                {
                    tab[i][j] |= static_cast<uint64_t>(1) << it;
                }
            }
            else
            {
                // alredy assign to 0
            }
        }
    }

    return tab;
}();

// non-type template param
template<Slider> struct MagicSlider;

/*---------SPECIALIZATIONS---------*/

template<> struct MagicSlider<Slider::Rook>
{
    [[nodiscard("PURE FUN")]] static constexpr uint64_t occupanciesMask(const int square)
    {
        return (inBetween[square][square + (7 - (square % 8))]        // right
                | inBetween[square][square - (square % 8)]            // left       
                | inBetween[square][square + 8 * (7 - (square / 8))]  // up
                | inBetween[square][square - 8 * (square / 8)]);      // down
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
        return (inBetween[square][square + 9 * std::min(7 - (square % 8), 7 - (square / 8))]  // up-right
             |  inBetween[square][square + 7 * std::min((square % 8), 7 - (square / 8))]      // up-left
             |  inBetween[square][square - 7 * std::min(7 - (square % 8), (square / 8))]      // down-right
             |  inBetween[square][square - 9 * std::min((square % 8), (square / 8))]);        // down-left
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
