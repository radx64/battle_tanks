#pragma once

#include "gui/Event.hpp"

// TODO move it to some utils
#define UNUSED(x) static_cast<void>(x)

// TODO think about recursive variadic macro expansion in the future :)
#define UNUSED2(x, y) static_cast<void>(x); static_cast<void>(y)
#define UNUSED3(x, y, z) UNUSED2(x,y); static_cast<void>(y)

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
    virtual EventStatus receive(const event::MouseMoved& mouseMovedEvent)
    {
        UNUSED(mouseMovedEvent);
        return EventStatus::NotConsumed; 
    };
    virtual EventStatus receive(const event::MouseButtonPressed& mouseButtonPressedEvent)
    {
        UNUSED(mouseButtonPressedEvent);
        return EventStatus::NotConsumed;
    };
    
    virtual EventStatus receive(const event::MouseButtonReleased& mouseButtonReleasedEvent)
    {
        UNUSED(mouseButtonReleasedEvent);
        return EventStatus::NotConsumed;
    };

    virtual EventStatus receive(const event::MouseEntered& mouseEnteredEvent)
    {
        UNUSED(mouseEnteredEvent);
        return EventStatus::NotConsumed;
    };

    virtual EventStatus receive(const event::MouseLeft& mouseLeftEvent)
    {
        UNUSED(mouseLeftEvent);
        return EventStatus::NotConsumed;
    };

    virtual EventStatus receive(const event::KeyboardKeyPressed& keyboardKeyPressed)
    {
        UNUSED(keyboardKeyPressed);
        return EventStatus::NotConsumed;
    }

    virtual EventStatus receive(const event::KeyboardKeyReleased& keyboardKeyReleased)
    {
        UNUSED(keyboardKeyReleased);
        return EventStatus::NotConsumed;
    }
};

}  // namespace gui

