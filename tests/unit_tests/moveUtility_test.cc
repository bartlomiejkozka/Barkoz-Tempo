#include <gtest/gtest.h>

#include "MoveUtils.hpp"


TEST(MoveGeneartorTest, BasicAssertions) 
{
    EXPECT_EQ(MoveUtils::inBetween[0][0], 0);
}