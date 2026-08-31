#pragma once

#include <SFML/Window/Mouse.hpp>

#include "engine/core/MouseCode.hpp"

namespace engine::backends
{

engine::core::MouseButton fromSf(const sf::Mouse::Button button)
{
    switch (button)
    {
        case sf::Mouse::Button::Left : return engine::core::MouseButton::Left;
        case sf::Mouse::Button::Right : return engine::core::MouseButton::Right;
        case sf::Mouse::Button::Middle : return engine::core::MouseButton::Middle;
        case sf::Mouse::Button::XButton1 : return engine::core::MouseButton::Extra1;
        case sf::Mouse::Button::XButton2 : return engine::core::MouseButton::Extra2;
        default : return engine::core::MouseButton::Extra2;
    }
}

engine::core::MouseWheel fromSf(const sf::Mouse::Wheel wheel)
{
    switch (wheel)
    {
        case sf::Mouse::Wheel::HorizontalWheel :  return engine::core::MouseWheel::Horizontal;
        case sf::Mouse::Wheel::VerticalWheel :  return engine::core::MouseWheel::Vertical;
        default : return engine::core::MouseWheel::Horizontal;
    }
}

}  // namespace engine::backends
