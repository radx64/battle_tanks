#pragma once

#include <memory>

#include "gui/style/Button.hpp"
#include "gui/style/EditBox.hpp"
#include "gui/style/ProgressBar.hpp"
#include "gui/style/Text.hpp"
#include "gui/style/Window.hpp"

namespace gui::style
{

struct Sheet
{
    Button button;
    Window window;
    Text label;
    ProgressBar progressBar;
    EditBox editBox;
};

}  // gui::style
