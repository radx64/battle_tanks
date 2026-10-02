#pragma once

#include <chrono>
#include <cstdint>

namespace engine
{

class Stopwatch
{
public:
    Stopwatch();

    std::int32_t restart();
    std::int32_t getElapsedMilliseconds() const;
    float getElapsedSeconds() const;

private:
    std::chrono::steady_clock::time_point start_;
};

}  // namespace engine
