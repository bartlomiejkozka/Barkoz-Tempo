// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
/* Generate code for arrays with magic numbers   */
/*************************************************/

#include "MagicGenerator.hpp"
#include "MagicSlider.hpp"
#include "HashMagicFunctions.hpp"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <array>
#include <utility>

void magicGenerate()
{
    std::array<FancyMagicFunction<Slider::Rook>, MAGIC_ARRAY_SIZE> magicsRook;
    std::array<FancyMagicFunction<Slider::Bishop>, MAGIC_ARRAY_SIZE> magicsBishop;
    
    for (int i = 0; i < MAGIC_ARRAY_SIZE; ++i)
    {
        magicsRook[i] = FancyMagicFunction<Slider::Rook>(i, count_1s(MagicSlider<Slider::Rook>::occupanciesMask(i)));
        magicsBishop[i] = FancyMagicFunction<Slider::Bishop>(i, count_1s(MagicSlider<Slider::Bishop>::occupanciesMask(i)));
    }

    auto print_table = [](const char* title, auto const& arr) {
        std::cout << "\n" << title << "\n";
        std::cout << "-----------------------------------------------\n";
        std::cout << std::left
                  << std::setw(6)  << "SQ"
                  << std::setw(22) << "MAGIC (hex)"
                  << std::setw(12) << "OFFSET"
                  << "\n";
        std::cout << "-----------------------------------------------\n";

        for (int sq = 0; sq < (int)arr.size(); ++sq) {
            const auto& e = arr[sq];
            // jeśli masz e.magic i e.offset jako pola, zamień e.magic() -> e.magic itp.
            const uint64_t magic  = e.getMagic();
            const int offset = e.getOffset();

            std::cout << std::left
                      << std::setw(6)  << sq
                      << "0x" << std::hex << std::setw(16) << std::setfill('0') << magic
                      << std::dec << std::setfill(' ')
                      << std::setw(6)  << "  "
                      << std::setw(12) << offset
                      << "\n";
        }
    };

    print_table("ROOK MAGICS",   magicsRook);
    print_table("BISHOP MAGICS", magicsBishop);

    {
        std::ofstream out("../out/magics.txt");
        if (out) 
        {
            auto dump = [&](const char* title, auto const& arr) {
                out << title << "\n";
                out << "SQ, MAGIC_HEX, OFFSET\n";
                for (int sq = 0; sq < (int)arr.size(); ++sq) {
                    const auto& e = arr[sq];
                    out << "{ 0x" << std::hex << e.getMagic() << "ULL"
                        << std::dec << ", " << e.getOffset() << "},\n";
                }
                out << "\n";
            };
            dump("ROOK MAGICS",   magicsRook);
            dump("BISHOP MAGICS", magicsBishop);
        }
        else
        {
            std::cout << "Writing magics to file failed!" << std::endl;
        }
    }
}
