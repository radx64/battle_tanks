#pragma once

#include <cstdint>

#include "engine/events/KeyEvent.hpp"

namespace engine::input {class KeyboardHandler;}

namespace engine::input
{

class TextEnteredReceiver
{
public:
    TextEnteredReceiver();
    virtual ~TextEnteredReceiver();

friend class KeyboardHandler;

protected:
    virtual void onTextEntered(const engine::events::TextEnteredEvent& event) = 0;
    void attach(KeyboardHandler* handler);

    KeyboardHandler* handler_;
};

}  // namespace engine::input
