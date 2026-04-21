#pragma once

#include <SFML/Graphics/Color.hpp>

#include "gui/style/Text.hpp"

namespace gui::style
{

struct ProgressBar
{
    sf::Color backgroundColor;
    sf::Color barColor;
    Text text;
    float borderThickness;
};

}  // gui::style
