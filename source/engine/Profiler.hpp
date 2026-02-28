#pragma once

#include <chrono>
#include <string_view>

#include "engine/math/Math.hpp"

namespace engine
{

struct ProfileResult
{
    std::string_view name;
    std::string_view unit;
    float lastFrame;
    float average;
};

class Profiler
{
public:
    Profiler(std::string_view name, std::string_view unit);

    void startFrame();
    void endFrame();
    const ProfileResult& getResult() const;

protected:
    std::string_view name_;
    std::string_view unit_;
    std::chrono::steady_clock::time_point startTime_{};
    std::chrono::steady_clock::time_point endTime_{};

    math::Average<float> average_;
    ProfileResult result_;
};

}  // engine
