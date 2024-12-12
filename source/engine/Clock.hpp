#pragma once

#include <chrono>
#include <cstdint>

namespace engine
{

struct Clock
{
    using rep = uint32_t;
    using period = std::ratio<1, 1000>; // 1 tick = 1 milisecond
    using duration = std::chrono::duration<rep, period>;
    using time_point = std::chrono::time_point<Clock>;

    static constexpr bool is_steady = false;       // Clock is not steady (as is engine driven and time might be slowed down)
};

};
