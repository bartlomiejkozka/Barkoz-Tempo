#include <gtest/gtest.h>

#include "MoveGeneration/MoveUtils.hpp"


TEST(MoveGeneartorTest, HorizontalPositions) 
{
    EXPECT_EQ(MoveUtils::inBetween[0][0], 0);
    EXPECT_EQ(MoveUtils::inBetween[0][2], 2);
    EXPECT_EQ(MoveUtils::inBetween[16][20], 0xE0000);
    EXPECT_EQ(MoveUtils::inBetween[56][63], 0x7E00000000000000);
}

TEST(MoveGeneratorTest, VerticalPositions)
{
    EXPECT_EQ(MoveUtils::inBetween[0][16], 1ULL << 8);
    EXPECT_EQ(MoveUtils::inBetween[8][40], (1ULL << 16) | (1ULL << 24) | (1ULL << 32));
    EXPECT_EQ(MoveUtils::inBetween[48][0], (1ULL << 8) | (1ULL << 16) | (1ULL << 24) | (1ULL << 32) | (1ULL << 40));
    EXPECT_EQ(MoveUtils::inBetween[3][59], 
        (1ULL << 8+3) | (1ULL << 16+3) | (1ULL << 24+3) | (1ULL << 32+3) | (1ULL << 40+3) | (1ULL << 48+3));
}

TEST(MoveGeneratorTest, DiagonalPositions)
{
    EXPECT_EQ(MoveUtils::inBetween[0][63],
        (1ULL << 9)  |
        (1ULL << 18) |
        (1ULL << 27) |
        (1ULL << 36) |
        (1ULL << 45) |
        (1ULL << 54));
    EXPECT_EQ(MoveUtils::inBetween[0][9], 0);
    EXPECT_EQ(MoveUtils::inBetween[0][18], 1ULL << 9);
    EXPECT_EQ(MoveUtils::inBetween[0][27], (1ULL << 9) | (1ULL << 18));
    EXPECT_EQ(MoveUtils::inBetween[7][49], (1ULL << 14) | (1ULL << 21) | (1ULL << 28) | (1ULL << 35) | (1ULL << 42));
    EXPECT_EQ(MoveUtils::inBetween[49][7], (1ULL << 14) | (1ULL << 21) | (1ULL << 28) | (1ULL << 35) | (1ULL << 42));
}

TEST(MoveGeneratorTest, UnusedinBetweenPositions)
{
    EXPECT_EQ(MoveUtils::inBetween[0][10], 0);
    EXPECT_EQ(MoveUtils::inBetween[49][0], 0);
    EXPECT_EQ(MoveUtils::inBetween[12][63], 0);
    EXPECT_EQ(MoveUtils::inBetween[63][2], 0);
}
