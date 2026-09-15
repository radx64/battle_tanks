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
engine::gui::EventStatus onButtonPressed(const engine::Vec2f&, const engine::core::MouseButton&, bool) override
{
    return engine::gui::EventStatus::NotConsumed;
}
engine::gui::EventStatus onButtonReleased(const engine::Vec2f& mouse_postion, const engine::core::MouseButton& button) override
{    
    if (button == engine::core::MouseButton::Left)
    {
        actionLeft_(mouse_postion.x, mouse_postion.y);
        return engine::gui::EventStatus::Consumed;
    }
    else if (button == engine::core::MouseButton::Right)
    {
        actionRight_(mouse_postion.x, mouse_postion.y);
        return engine::gui::EventStatus::Consumed;
    }
    return engine::gui::EventStatus::NotConsumed;
}

engine::gui::EventStatus onMouseMoved(const engine::Vec2f&) override
{
    return engine::gui::EventStatus::NotConsumed;
}

engine::gui::EventStatus onMouseScrolled(const engine::Vec2f&, const engine::core::MouseWheel&, const float) override
{
    return engine::gui::EventStatus::NotConsumed;
}


std::function<void(float, float)> actionLeft_;
std::function<void(float, float)> actionRight_;

};

}  // namespace sandbox
