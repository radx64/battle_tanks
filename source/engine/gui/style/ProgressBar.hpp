#pragma once

#include <SFML/Graphics/Color.hpp>

#include "engine/gui/style/Text.hpp"

namespace engine::gui::style
{

struct ProgressBar
{
    sf::Color backgroundColor;
    sf::Color barColor;
    Text text;
    float borderThickness;
};

}  // engine::gui::style
