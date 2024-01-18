#ifndef ENGINE_FPSCOUNTER_HPP_
#define ENGINE_FPSCOUNTER_HPP_

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

#endif  // ENGINE_FPSCOUNTER_HPP_