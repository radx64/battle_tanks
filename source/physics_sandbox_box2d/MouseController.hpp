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
engine::gui::EventStatus onButtonPressed(const sf::Vector2f&, const sf::Mouse::Button&, bool) override
{
    return engine::gui::EventStatus::NotConsumed;
}
engine::gui::EventStatus onButtonReleased(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button) override
{    
    if (button == sf::Mouse::Button::Left)
    {
        actionLeft_(mouse_postion.x, mouse_postion.y);
        return engine::gui::EventStatus::Consumed;
    }
    else if (button == sf::Mouse::Button::Right)
    {
        actionRight_(mouse_postion.x, mouse_postion.y);
        return engine::gui::EventStatus::Consumed;
    }
    return engine::gui::EventStatus::NotConsumed;
}

engine::gui::EventStatus onMouseMoved(const sf::Vector2f&) override
{
    return engine::gui::EventStatus::NotConsumed;
}

std::function<void(float, float)> actionLeft_;
std::function<void(float, float)> actionRight_;

};

}  // namespace sandbox
