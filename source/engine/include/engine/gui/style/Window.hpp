#pragma once

#include <SFML/Graphics/Color.hpp>

#include "engine/gui/style/Text.hpp"

namespace engine::gui::style
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

}  // engine::gui::style
