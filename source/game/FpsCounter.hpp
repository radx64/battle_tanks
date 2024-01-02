#ifndef GAME_FPSCOUNTER_HPP_
#define GAME_FPSCOUNTER_HPP_

#include <SFML/System.hpp>

#include <cstdint>

#include <iostream>

namespace game
{

class FpsCounter
{
public:
    FpsCounter() : fps_(0.f), smoothing_{0.1f}
    {
    }

    void startMeasurement()
    {
        clock_.restart();
    }

    void endMeasurement()
    {
        auto measurement = clock_.getElapsedTime().asMilliseconds();

        if (measurement <= 1)
        {
            fps_ = 1000.f; //infitite 🤣
        }
        else
        {
            float calculatedFps = 1000.f/static_cast<float>(measurement);
            fps_ = smoothing_ * calculatedFps + (1.0 - smoothing_) * fps_;
        }
    }

    float getFps()
    {
        return fps_;
    }

protected:
    sf::Clock clock_;
    float fps_;
    float smoothing_;
};

}  // namespace game

#endif  // GAME_FPSCOUNTER_HPP_