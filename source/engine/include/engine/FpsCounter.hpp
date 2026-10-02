#pragma once

#include <cstdint>

#include "engine/Stopwatch.hpp"

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
    engine::Stopwatch clock_;
    uint32_t fps_;
    float smoothing_;
};

}  // namespace engine
