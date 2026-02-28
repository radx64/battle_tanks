#include "engine/FpsCounter.hpp"

namespace engine
{

FpsCounter::FpsCounter() : fps_(0.f), smoothing_{0.1f}
{}

void FpsCounter::startMeasurement()
{
    clock_.restart();
}

void FpsCounter::endMeasurement()
{
    auto measurement = clock_.getElapsedTime().asMilliseconds();

    if (measurement <= 1)
    {
        fps_ = 1000.f; //infitite 🤣
    }
    else
    {
        float calculatedFps = 1000.f / static_cast<float>(measurement);
        fps_ = static_cast<uint32_t>(smoothing_ * calculatedFps + (1.0 - smoothing_) * fps_);
    }
}

uint32_t FpsCounter::getFps()
{
    return fps_;
}

}  // namespace engine
