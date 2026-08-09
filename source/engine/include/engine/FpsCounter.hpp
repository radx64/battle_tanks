#pragma once

#include <SFML/System.hpp>

#include <cstdint>

namespace engine
{

class FpsCounter
{
public:
    FpsCounter();
    void startMeasurement();
    void endMeasurement();
    uint32_t getFps();

protected:
    sf::Clock clock_;
    uint32_t fps_;
    float smoothing_;
};

}  // namespace engine
