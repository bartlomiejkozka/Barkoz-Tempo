// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
/* Rook moves Map using magics                   */
/*************************************************/

#ifndef ROOK_MAP
#define ROOK_MAP
// Copyright (c) 2025 Bartlomiej Kozka

#include <cstdint>
#include <array>
#include <utility>
#include <bit>

#include "MoveUtils.hpp"
#include "BitOperation.hpp"

//TODO: change the fun. in King/Knight to return the possible squares not template
        

class Rook 
{
public:
    //--------------------
    // Initilizers
    //--------------------
    
    Rook() = delete;
    
    //------------------
    // Main API function
    //------------------
    // TODO: Make Template for these slider functions
    /*
    * This function could be also used to get attacksTo squares, then originSq = attacksTo, and bbUs = bbThem, and bbThem = bbUs
    */
    [[nodiscard("PURE FUN")]] static uint64_t getMoves(const int originSq, const uint64_t bbUs, const uint64_t bbThem)
    {
        if (originSq < 0 || originSq > 63) return 0ULL;

        const uint64_t occ = (bbUs | bbThem) & occupanciesMask(originSq);

        const uint64_t attacks = RookAttacks[originSq][MoveUtils::Slider::transform(occ, RookMagics[originSq])];

        return attacks & ~bbUs;
    }

private:
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

    static constexpr std::array<std::pair<uint64_t, int>, 64> RookMagics = {
        {
            { 0x80008040002010ULL, 12},
            { 0x80200040008011ULL, 11},
            { 0x200102040820008ULL, 11},
            { 0x880051000880080ULL, 11},
            { 0x4a00100408860060ULL, 11},
            { 0x2200240861100200ULL, 11},
            { 0x3000100040d8200ULL, 11},
            { 0x8200084c01008022ULL, 12},
            { 0x24800020400880ULL, 11},
            { 0x800402000401000ULL, 10},
            { 0x802000100088ULL, 10},
            { 0x2405800804805000ULL, 10},
            { 0x2881802400080080ULL, 10},
            { 0x10800400804200ULL, 10},
            { 0xd001000200040100ULL, 10},
            { 0xc0800100004080ULL, 11},
            { 0x2080208000904000ULL, 11},
            { 0x202000c000403000ULL, 10},
            { 0x130808020001001ULL, 10},
            { 0x4904650010005900ULL, 10},
            { 0x2020004082010ULL, 10},
            { 0x4412808004010200ULL, 10},
            { 0x1400040008821001ULL, 10},
            { 0x608020000408114ULL, 11},
            { 0x4350208880004006ULL, 11},
            { 0x11100040c0006000ULL, 10},
            { 0x200100480200180ULL, 10},
            { 0x2001180180100080ULL, 10},
            { 0x2000100500080100ULL, 10},
            { 0xa0040080800200ULL, 10},
            { 0x802100400010288ULL, 10},
            { 0x2006200008401ULL, 11},
            { 0x804008800822ULL, 11},
            { 0x800402000401000ULL, 10},
            { 0x1809004802004ULL, 10},
            { 0xa45001001000a20ULL, 10},
            { 0x8204004008080080ULL, 10},
            { 0xa0040080800200ULL, 10},
            { 0x60a0020184000810ULL, 10},
            { 0x2306404082000401ULL, 11},
            { 0x8080004020004000ULL, 11},
            { 0x400081010020ULL, 10},
            { 0x211001020010040ULL, 10},
            { 0x6002900500210009ULL, 10},
            { 0x200c000800110100ULL, 10},
            { 0x1000040002008080ULL, 10},
            { 0xa011042040088ULL, 10},
            { 0x4002040080420021ULL, 11},
            { 0x4000a9c280110100ULL, 11},
            { 0x4410004000200040ULL, 10},
            { 0x2801000200480ULL, 10},
            { 0x400a0010204200ULL, 10},
            { 0x2000100500080100ULL, 10},
            { 0x1000040002008080ULL, 10},
            { 0x20801100400ULL, 10},
            { 0x2200304104840200ULL, 11},
            { 0x4010ad008001ULL, 12},
            { 0xc0400010210083ULL, 11},
            { 0x400090012200141ULL, 11},
            { 0x20201000080501ULL, 11},
            { 0x800a080010030301ULL, 11},
            { 0x10200680b100402ULL, 11},
            { 0xa1001100800b204ULL, 11},
            { 0x320040500209142ULL, 12}
        }
    };

    inline static const std::array<std::array<uint64_t, 4096>, 64> RookAttacks = [] () {
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
                int idx = MoveUtils::Slider::transform(occupanies[i], RookMagics[sq].first, RookMagics[sq].second);
                attacks[sq][idx] = attackers[i];
            }
        }
        return attacks;
    }();
};

#endif // ROOK_MAP
