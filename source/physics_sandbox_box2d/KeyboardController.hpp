#pragma once

#include <functional>

#include "engine/input/KeyboardReceiver.hpp"

namespace sandbox
{

class KeyboardController : public engine::input::KeyboardReceiver
{
public:
KeyboardController (std::function<void()> actionSpace)
:actionSpace_(actionSpace)
{}

protected:
void onKeyPressed(const engine::events::KeyPressedEvent& keyEvent) override
{
    (void) keyEvent;
    actionSpace_();
}
void onKeyReleased(const engine::events::KeyReleasedEvent& keyEvent) override
{  
    (void) keyEvent;
}

std::function<void()> actionSpace_;

};

}  // namespace sandbox
