#include "engine/Profiler.hpp"

#include <chrono>
#include <cassert>

namespace engine
{
Profiler::Profiler(std::string_view name, std::string_view unit)
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
    auto diff = std::chrono::duration_cast<std::chrono::microseconds>(endTime_ - startTime_).count();
    assert(diff >= 0);

    auto diff_ms = diff / 1000.f;
    result_.lastFrame = diff_ms;
    result_.average = average_.calculate(diff_ms);
}

const ProfileResult& Profiler::getResult() const
{
    return result_;
}
    
}  // namespace engine
