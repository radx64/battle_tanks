#pragma once

#include "engine/input/MouseReceiver.hpp"
#include "engine/Scene.hpp"

#include "Circle.hpp"

namespace sandbox
{

class MouseController : public engine::input::MouseReceiver
{
public:
MouseController (engine::Scene& scene)
:scene_(scene)
{}

protected:
engine::gui::EventStatus onButtonPressed(const engine::Vec2f& , const engine::core::MouseButton&, bool) override
{
    return engine::gui::EventStatus::NotConsumed;
}
engine::gui::EventStatus onButtonReleased(const engine::Vec2f& mouse_postion, const engine::core::MouseButton& button) override
{
    if (button != engine::core::MouseButton::Left)
    {
        return engine::gui::EventStatus::NotConsumed;
    }

    scene_.spawnObject(std::make_unique<Circle>(
        rand(),
        mouse_postion.x,
        mouse_postion.y,
        25,
        100,
        engine::RigidBody::Type::DYNAMIC));

    return engine::gui::EventStatus::Consumed;
}

engine::gui::EventStatus onMouseMoved(const engine::Vec2f&) override
{
    return engine::gui::EventStatus::NotConsumed;
}

engine::gui::EventStatus onMouseScrolled(const engine::Vec2f&, const engine::core::MouseWheel&, const float) override
{
    return engine::gui::EventStatus::NotConsumed;
}


engine::Scene& scene_;

};

}  // namespace sandbox
