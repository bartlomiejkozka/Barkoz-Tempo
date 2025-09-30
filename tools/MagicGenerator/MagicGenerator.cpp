// Copyright (c) 2025 Bartlomiej Kozka
// All rights reserved.

/*************** File description ****************/
/* Generate code for arrays with magic numbers   */
/*************************************************/

#include "MagicGenerator.hpp"
#include "MagicSlider.hpp"
#include "HashMagicFunctions.hpp"

#include <array>
#include <utility>

void generatePlainText()
{
    
}

void magicGenerate()
{
    std::array<FancyMagicFunction<Slider::Rook>, MAGIC_ARRAY_SIZE> magicsRook;
    std::array<FancyMagicFunction<Slider::Bishop>, MAGIC_ARRAY_SIZE> magicsBishop;
    
    for (int i = 0; i < MAGIC_ARRAY_SIZE; ++i)
    {
        magicsRook[i] = FancyMagicFunction<Slider::Rook>(i, count1s(MagicSlider<Slider::Rook>::occupanciesMask(i)));
        magicsBishop[i] = FancyMagicFunction<Slider::Bishop>(i, count1s(MagicSlider<Slider::Bishop>::occupanciesMask(i)));
    }


}
