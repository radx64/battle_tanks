#pragma once

#include <SFML/Graphics.hpp>

namespace gui::slider
{

class VerticalMousePolicy
{
public:
    static float translatePositionToThumbValue(const sf::Vector2f& mouse_position, const sf::Vector2f& trackPosition,
        const sf::Vector2f& trackSize, const sf::Vector2f& thumbSize, const float min, const float max, const float step);
};

}  // namespace gui::slider
