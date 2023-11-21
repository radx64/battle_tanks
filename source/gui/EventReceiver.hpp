#ifndef GUI_EVENTCONSUMER_HPP_
#define GUI_EVENTCONSUMER_HPP_

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
};

}  // namespace gui

#endif  // GUI_EVENTCONSUMER_HPP_