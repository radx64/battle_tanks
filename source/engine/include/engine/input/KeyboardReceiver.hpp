#pragma once

#include "engine/events/KeyEvent.hpp"

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
    virtual void onKeyPressed(const engine::events::KeyPressed& keyEvent) = 0;
    virtual void onKeyReleased(const engine::events::KeyReleased& keyEvent) = 0;

    // attach is used by keyboardhandler to properly setup destruction of receiver
    void attach(KeyboardHandler* handler);
    KeyboardHandler* handler_;
};
    
}  // namespace engine::input
