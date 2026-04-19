#pragma once

#include <SFML/Graphics/Color.hpp>

#include "gui/style/Text.hpp"

namespace gui::style
{

struct Window
{
    sf::Color background;
    sf::Color inactiveBackground;

    sf::Color header;
    sf::Color inactiveHeader;

    float borderThickness = 1.f;
    sf::Color borderColor = sf::Color::Black;

    Text titleText;
};

}  // gui::style
