#pragma once

#include <fmt/format.h>

#include "engine/events/Event.hpp"
#include "engine/core/MouseCode.hpp"

#include "engine/Vector2.hpp"

namespace engine::events
{

class MouseEvent : public Event
{
public:
    const engine::Vec2i& getPosition() const
    {
        return position_;
    }

    EVENT_CATEGORY(EventCategory::Mouse | EventCategory::Input);

protected:
    MouseEvent(const engine::Vec2i position)
    : position_(position)
    {}

    engine::Vec2i position_;
};


class MouseMovedEvent : public MouseEvent
{
public:
    MouseMovedEvent(const engine::Vec2i position)
    : MouseEvent(position)
    {}

    std::string toString() const override
    {
        return fmt::format("MouseMovedEvent: position={}", engine::toString(position_));
    }

    EVENT_TYPE(MouseMoved);
};

class MouseScrolledEvent : public MouseEvent
{
public:
    MouseScrolledEvent(const engine::Vec2i& position, const engine::core::MouseWheel wheel, const float offset)
    : MouseEvent(position)
    , wheel_(wheel)
    , offset_(offset)
    {}

    const engine::core::MouseWheel& getWheel() const
    {
        return wheel_;
    }

    float getOffset() const
    {
        return offset_;
    }

    std::string toString() const override
    {
        return fmt::format("MouseScrolledEvent: position={}, wheel={}, offset={}", engine::toString(position_), core::toString(wheel_), offset_);
    } 

    EVENT_TYPE(MouseScrolled);

private:
    engine::core::MouseWheel wheel_;
    float offset_;
};

class MouseButtonEvent : public MouseEvent
{
public:
    engine::core::MouseButton getButton() const
    {
        return button_;
    }

    std::string toString() const override
    {
        return fmt::format("{}: position={}, button={}", getName(), engine::toString(position_), core::toString(button_));
    } 

    EVENT_CATEGORY(EventCategory::Mouse | EventCategory::Input | EventCategory::MouseButton);

protected:
    MouseButtonEvent(const engine::Vec2i& position, const engine::core::MouseButton button)
    : MouseEvent(position)
    , button_(button)
    {}

    engine::core::MouseButton button_;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public: 
    MouseButtonPressedEvent(const engine::Vec2i& position, const engine::core::MouseButton button)
    : MouseButtonEvent(position, button)
    {}

    EVENT_TYPE(MouseButtonPressed);
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public: 
    MouseButtonReleasedEvent(const engine::Vec2i& position, const engine::core::MouseButton button)
    : MouseButtonEvent(position, button)
    {}

    EVENT_TYPE(MouseButtonReleased);
};

}
