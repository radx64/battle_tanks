#include "engine/Profiler.hpp"

#include <chrono>

namespace engine
{
Profiler::Profiler(const char* name, const char* unit)
: name_{name}
, unit_{unit}
, average_{20}
, result_{}
{  
    result_.name = name_;
    result_.unit = unit_;
}
void Profiler::startFrame()
{
    startTime_ = std::chrono::steady_clock::now();
}
void Profiler::endFrame()
{
    endTime_ = std::chrono::steady_clock::now();
    assert(endTime_  >= startTime_);
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(endTime_ - startTime_).count();
    assert(diff >= 0);

    result_.lastFrame = diff;
    result_.average = average_.calculate(diff);
}

const ProfileResult& Profiler::getResult() const
{
    return result_;
}
    
}  // namespace engine
