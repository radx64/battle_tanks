#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "engine/Clock.hpp"

namespace engine
{

TEST(ClockShould, beAbleConvertToRawValuesFromDuration)
{
    auto some_time = Clock::time_point(Clock::duration(1000));
    uint32_t raw_time = some_time.time_since_epoch().count();
    EXPECT_EQ(raw_time, 1000);
}

TEST(ClockShould, workWithDifferentLiterals)
{
    using namespace std::literals;
    auto some_time = Clock::time_point(200ms);
    uint32_t raw_time = some_time.time_since_epoch().count();
    EXPECT_EQ(raw_time, 200);

    auto some_other_time = Clock::time_point(42s);
    uint32_t other_raw_time = some_other_time.time_since_epoch().count();
    EXPECT_EQ(other_raw_time, 42*1000);  

    auto some_other_other_time = Clock::time_point(1h);
    uint32_t other_other_raw_time = some_other_other_time.time_since_epoch().count();
    EXPECT_EQ(other_other_raw_time, 1 * 60 * 60 * 1000);  
}

}  // namespace engine
