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

    // TODO make some macro to generate below empty implenentation
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

    virtual EventStatus receive(const event::TextEntered& textEntered)
    {
        UNUSED(textEntered);
        return EventStatus::NotConsumed;
    }
};

}  // namespace gui
