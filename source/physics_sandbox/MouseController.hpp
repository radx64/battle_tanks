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
engine::gui::EventStatus onButtonPressed(const sf::Vector2f&, const sf::Mouse::Button&, bool) override
{
    return engine::gui::EventStatus::NotConsumed;
}
engine::gui::EventStatus onButtonReleased(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button) override
{
    if (button != sf::Mouse::Button::Left)
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

engine::gui::EventStatus onMouseMoved(const sf::Vector2f&) override
{
    return engine::gui::EventStatus::NotConsumed;
}

engine::Scene& scene_;

};

}  // namespace sandbox
