#include "game/FpsLimiter.hpp"

namespace game
{

FpsLimiter::FpsLimiter() : frameLimit_(60)
{
}

void FpsLimiter::startNewFrame()
{
    clock_.restart();
}

void FpsLimiter::setFrameLimit(uint32_t frameLimit)
{
    frameLimit_ = frameLimit;
}

void FpsLimiter::wait()
{
    auto elapsed = clock_.getElapsedTime().asMilliseconds();
    sf::Int32 timeToWait = (1000.f / static_cast<float>(frameLimit_)) - elapsed;
    if (timeToWait > 0) sf::sleep(sf::milliseconds(timeToWait));
}

}  // namespace game
