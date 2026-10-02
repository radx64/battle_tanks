#include <chrono>
#include <thread>

#include <gtest/gtest.h>

#include "engine/Stopwatch.hpp"

namespace engine
{

TEST(StopwatchTests, shouldStartNearZero)
{
    const Stopwatch stopwatch;

    EXPECT_LT(stopwatch.getElapsedMilliseconds(), 50);
    EXPECT_GE(stopwatch.getElapsedMilliseconds(), 0);
    EXPECT_NEAR(stopwatch.getElapsedSeconds(), stopwatch.getElapsedMilliseconds() / 1000.f, 0.001f);
}

TEST(StopwatchTests, shouldReportElapsedTimeAndResetOnRestart)
{
    Stopwatch stopwatch;
    std::this_thread::sleep_for(std::chrono::milliseconds{40});

    const auto elapsed = stopwatch.getElapsedMilliseconds();
    EXPECT_GE(elapsed, 30);
    EXPECT_NEAR(stopwatch.getElapsedSeconds(), static_cast<float>(elapsed) / 1000.f, 0.05f);

    const auto restarted = stopwatch.restart();
    EXPECT_GE(restarted, elapsed);
    EXPECT_LT(stopwatch.getElapsedMilliseconds(), 30);
}

}  // namespace engine
