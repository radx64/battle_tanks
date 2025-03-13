#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "engine/Concat.hpp"

namespace engine
{

TEST(ConcatShould, concatenateTwoStrings)
{
    auto result = concat("first", "second");
    std::cout << "returned: " << result << std::endl;
    EXPECT_STREQ(result.c_str(), "firstsecond");
}

TEST(ConcatShould, concatenateOneEmptyStringWithOneNotEmpty)
{
    auto result = concat("", "second");
    EXPECT_STREQ(result.c_str(), "second");

    result = concat("first", "");
    EXPECT_STREQ(result.c_str(), "first");
}

TEST(ConcatShould, concatenateTwoEmptyStrings)
{
    auto result = concat("", "");
    EXPECT_STREQ(result.c_str(), "");
}

}  // namespace engine
