#ifndef HASH_MAGIC_FUNCTIONS_HPP
#define HASH_MAGIC_FUNCTIONS_HPP

#include <cstdint>
#include <chrono>
#include <random>
#include <utility>
#include <bit>

#include "MoveUtils.hpp"
#include "BitOperation.hpp"
#include "Debug.h"

//-------------
// Local macros
//-------------
#define MAGIC_GEN_TIMEOUT		100000000
#define TWO_MOST_SIGN_BITSS		0xFF00000000000000ULL
#define RANDOM_SEED				416587

//-------------------
// enums declarations
//-------------------
enum class MagicHolder
{
	Rook,
	Bishop
};

// TODO:
//        
// - Probably in another TU, global defintion of attackTab, then in Magic Pieces pointer to first attackTab bitboard.
// - Do abstraction to MagicHolders

/*
* The Class concept is dedicated to
* generate random unique index for magic boards by brude force method - "Trial and Error"
* hold magic number and offset for specific bishop/rook position
*/
template<class randomGenerator = std::mt19937_64>
class FancyMagicFunction {

    // [magic number, offset]
    using FancyPair = std::pair<uint64_t, uint8_t>;

    public:
    
    //--------------------
    // Contants
    //--------------------
    static constexpr int maxIndex = 4096;   // maxium combination number of occupancies per square
  
    //--------------------
    // Intilizators
    //--------------------
    
    explicit FancyMagicFunction(const uint64_t square, const int bits, MagicHolder piece)
        : _magic(findMagic(square, bits, piece)), _offset(bits) {}
    
    // constexpr FancyMagicFunction(const FancyPair &params):
    //     : _magic(magic), _offset(offset) {} 

    // constexpr FancyMagicFunction(const uint64_t magic, const uint8_t offset)
    //     : _magic(magic), _offset(offset) {}

    //--------------------
    // Getters
    //--------------------

    constexpr uint64_t getMagic()    { return _magic; }
    constexpr uint8_t  getOffset()   { return _offset; }

    //--------------------

    constexpr uint64_t rmask(const int square)
    {
        return (MoveUtils::inBetween[square][square + (7 - (square % 8))]        // right
                | MoveUtils::inBetween[square][square - (square % 8)]            // left       
                | MoveUtils::inBetween[square][square + 8 * (7 - (square / 8))]  // up
                | MoveUtils::inBetween[square][square - 8 * (square / 8)]);      // down
    }

    constexpr uint64_t bmask(const int square)
    {    
        return (MoveUtils::inBetween[square][square + 9 * std::min(7 - (square % 8), 7 - (square / 8))]  // up-right
             |  MoveUtils::inBetween[square][square + 7 * std::min((square % 8), 7 - (square / 8))]      // up-left
             |  MoveUtils::inBetween[square][square - 7 * std::min(7 - (square % 8), (square / 8))]      // down-right
             |  MoveUtils::inBetween[square][square - 9 * std::min((square % 8), (square / 8))]);        // down-left
    }

    // computes one of occupanices combination based on occupancie index - max combinations=4096
    // * param[in] : bits -> occupaniec number - max=12
    // * param[in] : mask -> occupancies mask
    /* chessprogramming */
    constexpr uint64_t indexToUint64(const int index, const int bits, uint64_t mask)
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

	constexpr int transform(const uint64_t mask, const uint64_t magic, const int bits)
	{
		return (int)((mask * magic) >> (64 - bits));
	}

    // Generate all possible attacks from sq
    /* chessprogramming */
    static constexpr uint64_t rAttacks(const int square, const uint64_t block)
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
    
    /* chessprogramming */
    static constexpr uint64_t bAttacks(const int square, const uint64_t block)
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

	uint64_t random_uint64() 
	{
		randomGenerator random(RANDOM_SEED);
		uint64_t u1, u2, u3, u4;
		u1 = (uint64_t)(random()) & 0xFFFF; u2 = (uint64_t)(random()) & 0xFFFF;
		u3 = (uint64_t)(random()) & 0xFFFF; u4 = (uint64_t)(random()) & 0xFFFF;
		return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
	}

	uint64_t random_uint64_fewbits()
	{
		return random_uint64() & random_uint64() & random_uint64();
	}

	uint64_t findMagic(int square, int bits, MagicHolder piece)
	{
		uint64_t mask, occupanies[4096], attackers[4096], used[4096], magic;
		int i, j, k, n, fail;
		randomGenerator random(RANDOM_SEED);

		uint64_t (*attacksMask)(int, uint64_t) = nullptr;

		switch (piece)
		{
			case MagicHolder::Rook:
				mask = rmask(square);
				attacksMask = &rAttacks;
				break;
			case MagicHolder::Bishop:
				mask = bmask(square);
				attacksMask = &bAttacks;
				break;
			default:
				// already holded by function arg
				break;
		}
		n = count_1s(mask);

		DEBUG_BOARD_INFO("before AttacksMap");

		for(i = 0; i < (1 << n); i++)
		{
			occupanies[i] = indexToUint64(i, n, mask);
			attackers[i] = attacksMask(square, occupanies[i]);
		}

		DEBUG_BOARD_INFO("after AttacksMap");

		for(k = 0; k < MAGIC_GEN_TIMEOUT; k++)
		{
			magic = random_uint64_fewbits();
			/* chessprogramming -> when too few bits on most siginicant bits -> porbably bad magic */
			if(count_1s((mask * magic) & TWO_MOST_SIGN_BITSS) < 6)
			{
				continue;
			}

			for(i = 0; i < 4096; i++) 
			{
				used[i] = 0ULL;
			}

			for(i = 0, fail = 0; !fail && i < (1 << n); i++) 
			{  
				j = transform(occupanies[i], magic, bits);
				if(used[j] == 0ULL) 
				{
					used[j] = attackers[i];
				}
				else if(used[j] != attackers[i]) 
				{
					fail = 1;
				}
			}
			if(!fail) 
			{
				return magic;
			}
		}
		return 0ULL;
	}

    private:

    uint64_t _magic;
    int _offset;
};


#endif
