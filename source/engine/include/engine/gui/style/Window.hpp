#pragma once

#include "engine/gfx/Color.hpp"
#include "engine/gui/style/Text.hpp"

namespace engine::gui::style
{

struct Window
{
    engine::gfx::Color background;
    engine::gfx::Color inactiveBackground;

    engine::gfx::Color header;
    engine::gfx::Color inactiveHeader;

    float borderThickness = 1.f;
    engine::gfx::Color borderColor = engine::gfx::Color::Black;

    Text titleText;
};

}  // engine::gui::style
