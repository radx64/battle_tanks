#pragma once

#include "engine/Unused.hpp"
#include "gui/Event.hpp"

namespace gui
{

enum class EventStatus
{
    Consumed,
    NotConsumed
};

class EventReceiver
{
public:
    virtual EventStatus receive(const event::MouseMoved& mouseMovedEvent);
    virtual EventStatus receive(const event::MouseButtonPressed& mouseButtonPressedEvent);
    virtual EventStatus receive(const event::MouseButtonDoublePressed& mouseButtonDoublePressedEvent);
    virtual EventStatus receive(const event::MouseButtonReleased& mouseButtonReleasedEvent);
    virtual EventStatus receive(const event::MouseEntered& mouseEnteredEvent);
    virtual EventStatus receive(const event::MouseLeft& mouseLeftEvent);

    virtual EventStatus receive(const event::KeyboardKeyPressed& keyboardKeyPressed);
    virtual EventStatus receive(const event::KeyboardKeyReleased& keyboardKeyReleased);
    virtual EventStatus receive(const event::TextEntered& textEntered);

    virtual EventStatus receive(const event::FocusChange& focusChange);
    virtual EventStatus receive(const event::FocusLost& FocusLost);
    virtual EventStatus receive(const event::FocusGained& focusGained);
};

}  // namespace gui
