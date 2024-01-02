#ifndef GAME_FPSLIMITER_HPP_
#define GAME_FPSLIMITER_HPP_

#include <SFML/System.hpp>

#include <cstdint>

#include <iostream>

namespace game
{

class FpsLimiter
{
public:
    FpsLimiter() : frameLimit_(60)
    {
    }

    void startNewFrame()
    {
        clock_.restart();
    }

    void setFrameLimit(uint32_t frameLimit)
    {
        frameLimit_ = frameLimit;
    }

    void wait()
    {
        auto elapsed = clock_.getElapsedTime().asMilliseconds();
        sf::Int32 timeToWait = (1000.f / static_cast<float>(frameLimit_)) - elapsed;
        if (timeToWait > 0) sf::sleep(sf::milliseconds(timeToWait));
    }

protected:
    uint32_t frameLimit_;
    sf::Clock clock_;
};

}  // namespace game

#endif  // GAME_FPSLIMITER_HPP_