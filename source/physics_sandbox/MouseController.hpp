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
gui::EventStatus onButtonPressed(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button, bool doubleClick) override
{
    UNUSED3(mouse_postion, button, doubleClick);
    return gui::EventStatus::NotConsumed;
}
gui::EventStatus onButtonReleased(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button) override
{
    if (button != sf::Mouse::Button::Left)
    {
        return gui::EventStatus::NotConsumed;
    }

    scene_.spawnObject(std::make_unique<Circle>(
        rand(), 
        mouse_postion.x,
        mouse_postion.y,
        25,
        100,
        engine::RigidBody::Type::DYNAMIC));

    return gui::EventStatus::Consumed;
}

gui::EventStatus onMouseMoved(const sf::Vector2f& mouse_postion) override
{
    UNUSED(mouse_postion);
    return gui::EventStatus::NotConsumed;
}

engine::Scene& scene_;

};

}  // namespace sandbox
