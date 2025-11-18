// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
/* Bishop moves Map using magics                 */
/*************************************************/

#ifndef BISHOP_MAP
#define BISHOP_MAP

#include <cstdint>
#include <array>
#include <utility>
#include <bit>

#include "MoveUtils.hpp"
#include "BitOperation.hpp"

class Bishop 
{
public:
    //--------------------
    // Initilizers
    //--------------------
    
    Bishop() = delete;
    
    //------------------
    // Main API function
    //------------------

    [[nodiscard("PURE FUN")]] static uint64_t getMoves(const int originSq, const uint64_t bbUs, const uint64_t bbThem)
    {
        const uint64_t occ = (bbUs | bbThem) & occupanciesMask(originSq);
        
        const uint64_t attacks = BishopAttacks[originSq][MoveUtils::Slider::transform(occ, BishopMagics[originSq])];

        return attacks & ~bbUs;
    }

private:
    [[nodiscard("PURE FUN")]] static constexpr uint64_t occupanciesMask(const int square)
    {
        return (MoveUtils::inBetween[square][square + 9 * std::min(7 - (square % 8), 7 - (square / 8))]  // up-right
             |  MoveUtils::inBetween[square][square + 7 * std::min((square % 8), 7 - (square / 8))]      // up-left
             |  MoveUtils::inBetween[square][square - 7 * std::min(7 - (square % 8), (square / 8))]      // down-right
             |  MoveUtils::inBetween[square][square - 9 * std::min((square % 8), (square / 8))]);        // down-left
    }

    // return: mask of normal moves & attacks
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

    static constexpr std::array<std::pair<uint64_t, int>, 64> BishopMagics = {
        {  
            { 0x11014200820200ULL, 6},
            { 0x1011940084004080ULL, 5},
            { 0x4440400440004ULL, 5},
            { 0x2004104200234000ULL, 5},
            { 0x22a2021090284800ULL, 5},
            { 0x85142024009240ULL, 5},
            { 0xd02480690500040ULL, 5},
            { 0x808050300400ULL, 6},
            { 0x50580a10242100ULL, 5},
            { 0x40050404105200ULL, 5},
            { 0xa5041102220881ULL, 5},
            { 0x50580a10242100ULL, 5},
            { 0x9060211220000ULL, 5},
            { 0x420010120504030ULL, 5},
            { 0x22a2021090284800ULL, 5},
            { 0x84104222112010ULL, 5},
            { 0x2490010802900410ULL, 5},
            { 0x8422022008200ULL, 5},
            { 0x4008001088024088ULL, 7},
            { 0x10c4000840400940ULL, 7},
            { 0xa904010e01210000ULL, 7},
            { 0x4241000210008400ULL, 7},
            { 0x1181201084012010ULL, 5},
            { 0x2040439202008403ULL, 5},
            { 0x9011450828202400ULL, 5},
            { 0x3210020210028208ULL, 5},
            { 0x84240302080605ULL, 7},
            { 0x108080000202020ULL, 9},
            { 0x900d840000812000ULL, 9},
            { 0x8204420108411000ULL, 7},
            { 0x8211004012084402ULL, 5},
            { 0x2040439202008403ULL, 5},
            { 0x2514124101210440ULL, 5},
            { 0x884042000854100ULL, 5},
            { 0x800108800501048ULL, 7},
            { 0x4241420082080080ULL, 9},
            { 0x540420021820080ULL, 9},
            { 0x82018014040a400ULL, 7},
            { 0x128480080010088ULL, 5},
            { 0x2c18111501002090ULL, 5},
            { 0x8042080388824000ULL, 5},
            { 0x68840420000204ULL, 5},
            { 0x440b040202000102ULL, 7},
            { 0x8e02c208011081ULL, 7},
            { 0x2c206009010080ULL, 7},
            { 0x44c02c080a100020ULL, 7},
            { 0x1011940084004080ULL, 5},
            { 0x450040080908020ULL, 5},
            { 0xd02480690500040ULL, 5},
            { 0x40842402028202ULL, 5},
            { 0x2c0020211110080ULL, 5},
            { 0x610005104880800ULL, 5},
            { 0x102002048801ULL, 5},
            { 0x400284010009ULL, 5},
            { 0x40050404105200ULL, 5},
            { 0x1011940084004080ULL, 5},
            { 0x808050300400ULL, 6},
            { 0x84104222112010ULL, 5},
            { 0x80a1c0104411084ULL, 5},
            { 0x80a1c0104411084ULL, 5},
            { 0x2000120042282200ULL, 5},
            { 0x80a8004821084080ULL, 5},
            { 0x50580a10242100ULL, 5},
            { 0x11014200820200ULL, 6}
        }
    };
    
    inline static const std::array<std::array<uint64_t, 4096>, 64> BishopAttacks = [] () {
        std::array<std::array<uint64_t, 4096>, 64> attacks{};

        //iterate through all squares
        for (int sq = 0; sq < 64; ++sq)
        {
            uint64_t occupanies[4096], attackers[4096];
            uint64_t mask = occupanciesMask(sq);
            int n = count_1s(mask);

            for(int i = 0; i < (1 << n); i++)
            {
                occupanies[i] = MoveUtils::Slider::indexToUint64(i, n, mask);
                attackers[i] = attacksMask(sq, occupanies[i]);
                int idx = MoveUtils::Slider::transform(occupanies[i], BishopMagics[sq].first, BishopMagics[sq].second);
                attacks[sq][idx] = attackers[i];
            }
        }
        return attacks;
    }();
};

#endif // BISHOP_MAP
