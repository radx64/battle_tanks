#ifndef GAME_FPSLIMITER_HPP_
#define GAME_FPSLIMITER_HPP_

#include <SFML/System.hpp>

#include <cstdint>

namespace game
{

class FpsLimiter
{
public:
    FpsLimiter();
    void startNewFrame();
    void setFrameLimit(uint32_t frameLimit);
    void wait();

protected:
    uint32_t frameLimit_;
    sf::Clock clock_;
};

}  // namespace game

#endif  // GAME_FPSLIMITER_HPP_