#pragma once

#include "engine/gfx/Color.hpp"

#include "engine/gui/style/Text.hpp"

namespace engine::gui::style
{

struct EditBox
{
    engine::gfx::Color backgroundColor;
    engine::gfx::Color focusedBackgroundColor;
    engine::gfx::Color selectionColor;
    Text text;
};

}  // engine::gui::style
