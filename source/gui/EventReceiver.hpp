#pragma once


#include "gui/Event.hpp"

#define UNUSED(x) static_cast<void>(x)

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
    virtual EventStatus receive(const event::MouseButtonPressed& mousePressedEvent)
    {
        UNUSED(mousePressedEvent);
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
};

}  // namespace gui

