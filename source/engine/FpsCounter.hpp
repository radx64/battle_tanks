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
    float getFps();

protected:
    sf::Clock clock_;
    float fps_;
    float smoothing_;
};

}  // namespace engine

