#pragma once

#include <memory>

#include "engine/gui/style/Button.hpp"
#include "engine/gui/style/EditBox.hpp"
#include "engine/gui/style/ProgressBar.hpp"
#include "engine/gui/style/Text.hpp"
#include "engine/gui/style/Window.hpp"

namespace engine::gui::style
{

struct Sheet
{
    Button button;
    Button flatButton;
    Window window;
    Text label;
    ProgressBar progressBar;
    EditBox editBox;
};

}  // engine::gui::style
