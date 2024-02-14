#ifndef ENGINE_INPUT_MOUSERECEIVER_HPP_
#define ENGINE_INPUT_MOUSERECEIVER_HPP_

#include <SFML/Window/Mouse.hpp>


namespace engine::input {class MouseHandler;}

namespace engine::input
{

class MouseReceiver
{
public:
    MouseReceiver();
    virtual ~MouseReceiver();

friend class MouseHandler;
protected:
    virtual void onButtonPressed(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button) = 0;
    virtual void onButtonReleased(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button) = 0;
    virtual void onMouseMoved(const sf::Vector2f& mouse_postion) = 0;

    // attach is used by mousehandler to properly setup destruction of receiver
    void attach(MouseHandler* handler);
    MouseHandler* handler_;
};
    
}  // namespace engine::input

#endif  // ENGINE_INPUT_MOUSERECEIVER_HPP_