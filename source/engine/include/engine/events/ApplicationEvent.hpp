#pragma once

#include "engine/events/Event.hpp"

namespace engine::events
{

class WindowClosedEvent : public Event
{
public:
    WindowClosedEvent() = default;
    EVENT_TYPE(WindowClosed);
    EVENT_CATEGORY(EventCategory::Application);
};

}  // namespace engine::events
