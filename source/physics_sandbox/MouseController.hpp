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
void onButtonPressed(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button) override
{
    (void) mouse_postion;
    (void) button;
}
void onButtonReleased(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button) override
{
    if (button == sf::Mouse::Button::Left)
    {
        scene_.spawnObject(std::make_unique<Circle>(
            rand(), 
            mouse_postion.x,
            mouse_postion.y,
            25,
            100,
            engine::RigidBody::Type::DYNAMIC));
    }
}
void onMouseMoved(const sf::Vector2f& mouse_postion) override
{
    (void) mouse_postion;
}

engine::Scene& scene_;

};

}  // namespace sandbox

