#pragma once

#include <SFML/Window/Event.hpp>

namespace engine::input {class KeyboardHandler;}

namespace engine::input
{

class KeyboardReceiver
{
public:
    KeyboardReceiver();
    virtual ~KeyboardReceiver();

friend class KeyboardHandler;
protected:
    virtual void onKeyPressed(const sf::Event::KeyEvent& keyEvent) = 0;
    virtual void onKeyReleased(const sf::Event::KeyEvent& keyEvent) = 0;

    // attach is used by keyboardhandler to properly setup destruction of receiver
    void attach(KeyboardHandler* handler);
    KeyboardHandler* handler_;
};
    
}  // namespace engine::input
