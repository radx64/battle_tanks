#pragma once

#include <cstdint>

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
    virtual void onTextEntered(const uint32_t unicode) = 0;
    void attach(KeyboardHandler* handler);

    KeyboardHandler* handler_;
};

}  // namespace engine::input