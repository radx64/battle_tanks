#pragma once

#include <SFML/Graphics.hpp>

#include <engine/Vector2.hpp>

namespace engine::gui::slider
{

class HorizontalMousePolicy
{
public:
    static float translatePositionToThumbValue(const engine::Vec2f& mouse_position, const sf::Vector2f& trackPosition,
        const sf::Vector2f& trackSize, const sf::Vector2f& thumbSize, const float min, const float max, const float step);
};

}  // namespace engine::gui::slider
