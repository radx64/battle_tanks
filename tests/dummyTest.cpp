#include <gtest/gtest.h>

class FooTest : public ::testing::Test {};

TEST_F(FooTest, shouldPass)
{
    EXPECT_TRUE(true);
}