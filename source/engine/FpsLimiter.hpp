#ifndef ENGINE_FPSLIMITER_HPP_
#define ENGINE_FPSLIMITER_HPP_

#include <SFML/System.hpp>

#include <cstdint>

namespace engine
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

}  // namespace engine

#endif  // ENGINE_FPSLIMITER_HPP_