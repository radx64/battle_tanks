#pragma once

#include "engine/gfx/Color.hpp"

#include "engine/gui/style/Text.hpp"

namespace engine::gui::style
{

struct ProgressBar
{
    engine::gfx::Color backgroundColor;
    engine::gfx::Color barColor;
    Text text;
    float borderThickness;
};

}  // engine::gui::style
