#pragma once

#include <memory>

#include "gui/style/Button.hpp"
#include "gui/style/Window.hpp"
#include "gui/style/Text.hpp"

namespace gui::style
{

struct Sheet
{
    Button button;
    Window window;
    Text label;
};

}  // gui::style
