#pragma once

#include <string>
#include <string_view>

#include "engine/core/Base.hpp"

namespace engine::events
{

enum class EventType
{
    None = 0,
    WindowClosed, WindowResized, WindowLostFocus, WindowGainedFocus, WindowMoved, WindowMouseEntered, WindowMouseLeft,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
    KeyPressed, KeyReleased, TextEntered,
};

using engine::core::bit;

enum EventCategory
{
    None = 0,
    Application = bit(0),
    Input       = bit(1),
    Keyboard    = bit(2),
    Mouse       = bit(3),
    MouseButton = bit(4),
};

#define EVENT_TYPE(type) static EventType getStaticType() { return EventType::type; }\
    virtual EventType getEventType() const override { return getStaticType(); }\
    virtual const std::string_view getName() const override { return #type; }

#define EVENT_CATEGORY(category) virtual int getEventCategory() const override { return category; }

class Event
{
public:
    virtual ~Event() = default;

    virtual EventType getEventType() const = 0;
    virtual int getEventCategory() const = 0;
    virtual const std::string_view getName() const = 0;
    virtual std::string toString() const { return std::string(getName()); }

    bool isInCategory(EventCategory category) const
    {
        return static_cast<uint32_t>(getEventCategory()) & static_cast<uint32_t>(category);
    }

    bool handled_ = false;
};

}  // namespace engine::events
