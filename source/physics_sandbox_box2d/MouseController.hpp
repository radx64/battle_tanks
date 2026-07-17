#pragma once

#include <functional>

#include "engine/input/MouseReceiver.hpp"

namespace sandbox
{

class MouseController : public engine::input::MouseReceiver
{
public:
MouseController (std::function<void(float, float)> actionLeft, std::function<void(float, float)> actionRight)
:actionLeft_(actionLeft), actionRight_(actionRight)
{}

protected:
gui::EventStatus onButtonPressed(const sf::Vector2f&, const sf::Mouse::Button&, bool) override
{
    return gui::EventStatus::NotConsumed;
}
gui::EventStatus onButtonReleased(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button) override
{    
    if (button == sf::Mouse::Button::Left)
    {
        actionLeft_(mouse_postion.x, mouse_postion.y);
        return gui::EventStatus::Consumed;
    }
    else if (button == sf::Mouse::Button::Right)
    {
        actionRight_(mouse_postion.x, mouse_postion.y);
        return gui::EventStatus::Consumed;
    }
    return gui::EventStatus::NotConsumed;
}

gui::EventStatus onMouseMoved(const sf::Vector2f&) override
{
    return gui::EventStatus::NotConsumed;
}

std::function<void(float, float)> actionLeft_;
std::function<void(float, float)> actionRight_;

};

}  // namespace sandbox
