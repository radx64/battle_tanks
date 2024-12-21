#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "engine/Timer.hpp"

using namespace std::literals;

namespace engine
{

TEST(TimerShould, returnConfiguredDelay)
{
    Timer timer{100ms, [](){}};
    EXPECT_EQ(timer.getDelay().count(), 100);
}

TEST(TimerShould, setNewDelay)
{
    Timer timer{100ms, [](){}};
    EXPECT_EQ(timer.getDelay().count(), 100);

    timer.setDelay({200ms});
    EXPECT_EQ(timer.getDelay().count(), 200);
}

}  // namespace engine
