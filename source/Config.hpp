#pragma once

#include <cstdint>

#include "engine/Clock.hpp"

struct Config
{
    static constexpr uint32_t WINDOW_WIDTH = 1920;
    static constexpr uint32_t WINDOW_HEIGHT = 1000;

    static constexpr auto DOUBLE_CLICK_RATE = std::chrono::milliseconds(200);
};
