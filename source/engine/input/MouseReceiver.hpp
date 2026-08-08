#pragma once
#include <SFML/Window/Mouse.hpp>

#include "engine/gui/EventReceiver.hpp"

namespace engine::input {class MouseHandler;}

namespace engine::input
{

class MouseReceiver
{
public:
    MouseReceiver();
    virtual ~MouseReceiver();

    MouseReceiver(const MouseReceiver&) = delete;
    MouseReceiver(const MouseReceiver&&) = delete;
    MouseReceiver& operator=(const MouseReceiver&) = delete;

friend class MouseHandler;

protected:
    virtual engine::gui::EventStatus onButtonPressed(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button, bool double_click) = 0;
    virtual engine::gui::EventStatus onButtonReleased(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button) = 0;
    virtual engine::gui::EventStatus onMouseMoved(const sf::Vector2f& mouse_postion) = 0;

    // attach is used by mousehandler to properly setup destruction of receiver
    void attach(MouseHandler* handler);
    MouseHandler* handler_;
};
    
}  // namespace engine::input
