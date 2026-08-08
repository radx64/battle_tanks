#pragma once

#include <SFML/Graphics/Color.hpp>

#include "engine/gui/style/Text.hpp"

namespace engine::gui::style
{

struct EditBox
{
    sf::Color backgroundColor;
    sf::Color focusedBackgroundColor;
    sf::Color selectionColor;
    Text text;
};

}  // engine::gui::style
