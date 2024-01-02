#ifndef GAME_FPSCOUNTER_HPP_
#define GAME_FPSCOUNTER_HPP_

#include <SFML/System.hpp>

#include <cstdint>

namespace game
{

class FpsCounter
{
public:
    FpsCounter();
    void startMeasurement();
    void endMeasurement();
    float getFps();

protected:
    sf::Clock clock_;
    float fps_;
    float smoothing_;
};

}  // namespace game

#endif  // GAME_FPSCOUNTER_HPP_