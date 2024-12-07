#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "engine/Timer.hpp"

namespace engine
{

TEST(TimerShould, returnConfiguredDelay)
{
    Timer timer(100.f, [](){});
    EXPECT_EQ(timer.getDelay(), 100.f);
}

}  // namespace engine
