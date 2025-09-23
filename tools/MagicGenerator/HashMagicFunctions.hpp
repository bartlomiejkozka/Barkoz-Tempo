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
#include "MagicHolder.hpp"

//-------------
// Local macros
//-------------
#define MAGIC_GEN_TIMEOUT		100000000
#define TWO_MOST_SIGN_BITSS		0xFF00000000000000ULL
#define RANDOM_SEED				416587

//-------------------
// enums declarations
//-------------------
// enum class MagicHolder
// {
// 	Rook,
// 	Bishop
// };

// TODO:
// - Probably in another TU, global defintion of attackTab, then in Magic Pieces pointer to first attackTab bitboard.

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
    
    explicit FancyMagicFunction(const uint64_t square, const int bits, MagicHolder *piece)
        : _magic(findMagic(square, bits, piece)), _offset(bits), _rng(RANDOM_SEED) {}
    
    //--------------------
    // Getters
    //--------------------

    constexpr uint64_t getMagic()   { return _magic; }
    constexpr int  getOffset()   	{ return _offset; }

    //--------------------

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

	/* chessprogramming */
	uint64_t randomUint64() 
	{
		uint64_t u1, u2, u3, u4;
		u1 = (uint64_t)(_rng()) & 0xFFFF; u2 = (uint64_t)(_rng()) & 0xFFFF;
		u3 = (uint64_t)(_rng()) & 0xFFFF; u4 = (uint64_t)(_rng()) & 0xFFFF;
		return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
	}

	/* chessprogramming */
	uint64_t randomUint64Fewbits()
	{
		return randomUint64() & randomUint64() & randomUint64();
	}

	uint64_t findMagic(int square, int bits, MagicHolder *piece)
	{
		uint64_t mask, occupanies[maxIndex], attackers[maxIndex], used[maxIndex], magic;
		int i, j, k, n, fail;

		mask = piece->occupanciesMask(square);
		n = count_1s(mask);

		DEBUG_BOARD_INFO("before AttacksMap");

		for(i = 0; i < (1 << n); i++)
		{
			occupanies[i] = indexToUint64(i, n, mask);
			attackers[i] = piece->attacksMask(square, occupanies[i]);
		}

		DEBUG_BOARD_INFO("after AttacksMap");

		for(k = 0; k < MAGIC_GEN_TIMEOUT; k++)
		{
			magic = randomUint64Fewbits();
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
    randomGenerator _rng;
    uint64_t _magic;
    int _offset;
};


#endif
