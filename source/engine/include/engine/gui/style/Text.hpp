#pragma once

#include <cstdint>
#include <string_view>

#include "engine/gfx/Color.hpp"

namespace engine::gui::style
{

struct Text
{
    std::string_view fontName;
    uint32_t size = 14;
    engine::gfx::Color color;
};

}  // engine::gui::style
