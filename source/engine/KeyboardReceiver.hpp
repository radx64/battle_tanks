#ifndef ENGINE_KEYBOARDRECEIVER_HPP_
#define ENGINE_KEYBOARDRECEIVER_HPP_

#include <SFML/Window/Keyboard.hpp>

namespace engine
{

class KeyboardHandler;

class KeyboardReceiver
{
public:
    virtual ~KeyboardReceiver();

friend class KeyboardHandler;
protected:
    virtual void onKeyPressed(const sf::Keyboard::Key key) = 0;
    virtual void onKeyReleased(const sf::Keyboard::Key key) = 0;

    // attach is used by keyboardhandler to properly setup destruction of receiver
    void attach(KeyboardHandler* handler);
    KeyboardHandler* handler_;
};
    
}  // namespace engine

#endif  // ENGINE_KEYBOARDRECEIVER_HPP_