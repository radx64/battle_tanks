#pragma once

#include <cassert>
#include <chrono>

#include "engine/math/Math.hpp"

namespace engine
{

struct ProfileResult
{
    const char* name;
    const char* unit;
    int32_t lastFrame;
    int32_t average;
};

class Profiler
{
public:
    Profiler(const char* name, const char* unit);

    void startFrame();
    void endFrame();
    const ProfileResult& getResult() const;

protected:
    const char* name_;
    const char* unit_;
    std::chrono::steady_clock::time_point startTime_{};
    std::chrono::steady_clock::time_point endTime_{};

    math::Average average_;
    ProfileResult result_;
};

}  // engine
