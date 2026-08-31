#pragma once

#include "engine/events/Event.hpp"

namespace engine::events
{

class EventDispatcher
{
public:
    EventDispatcher(Event& event)
    : event_(event)
    {}

    template<typename T, typename F>
    bool dispatch(const F& func)
    {
        if (event_.getEventType() == T::getStaticType())
        {
            event_.handled_ |= func(static_cast<T&>(event_));
            return true;
        }
        return false;
    }

private:
    Event& event_;

};

}  // namespace engine::events
