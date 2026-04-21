#pragma once

#include <SFML/Graphics/Color.hpp>

#include "gui/style/Text.hpp"

namespace gui::style
{

struct EditBox
{
    sf::Color backgroundColor;
    sf::Color focusedBackgroundColor;
    sf::Color selectionColor;
    Text text;
};

}  // gui::style
