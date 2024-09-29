#pragma once
#include <SFML/Window/Mouse.hpp>

#include <gui/EventReceiver.hpp>

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
    virtual gui::EventStatus onButtonPressed(const sf::Vector2f& mousePostion, const sf::Mouse::Button& button) = 0;
    virtual gui::EventStatus onButtonReleased(const sf::Vector2f& mousePostion, const sf::Mouse::Button& button) = 0;
    virtual gui::EventStatus onMouseMoved(const sf::Vector2f& mousePostion) = 0;

    // attach is used by mousehandler to properly setup destruction of receiver
    void attach(MouseHandler* handler);
    MouseHandler* handler_;
};
    
}  // namespace engine::input
