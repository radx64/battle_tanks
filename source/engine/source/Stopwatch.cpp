#include "engine/Stopwatch.hpp"

namespace engine
{

Stopwatch::Stopwatch()
{
    restart();
}

std::int32_t Stopwatch::restart()
{
    const auto now = std::chrono::steady_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_).count();
    start_ = now;
    return static_cast<std::int32_t>(elapsed);
}

std::int32_t Stopwatch::getElapsedMilliseconds() const
{
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start_).count();
    return static_cast<std::int32_t>(elapsed);
}

float Stopwatch::getElapsedSeconds() const
{
    return static_cast<float>(getElapsedMilliseconds()) / 1000.f;
}

}  // namespace engine
