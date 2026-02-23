#include "engine/Profiler.hpp"

namespace engine
{
Profiler::Profiler(sf::Clock& clock, std::string_view name)
: name_{name}
, clock_{clock}
, average_{50}
{  
    
}
void Profiler::startFrame()
{
    frameStartTime_ = clock_.getElapsedTime();
}
void Profiler::endFrame()
{
    frameEndTime_ = clock_.getElapsedTime();
    assert(frameEndTime_  > frameStartTime_);

    auto diff = frameEndTime_ - frameStartTime_;
    lastFrame_ = diff.asMilliseconds();
    lastAverage_ = average_.calculate(lastFrame_);
}

int32_t Profiler::getLastFrame() const
{
    return lastFrame_;
}

int32_t Profiler::getLastAverage() const
{
    return lastAverage_;
}    
    
}
