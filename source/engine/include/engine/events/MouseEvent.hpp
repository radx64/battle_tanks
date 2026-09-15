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
    const engine::Vec2f& getPosition() const
    {
        return position_;
    }

    EVENT_CATEGORY(EventCategory::Mouse | EventCategory::Input);

protected:
    MouseEvent(const engine::Vec2f position)
    : position_(position)
    {}

    engine::Vec2f position_;
};


class MouseMoved : public MouseEvent
{
public:
    MouseMoved(const engine::Vec2f position)
    : MouseEvent(position)
    {}

    std::string toString() const override
    {
        return fmt::format("MouseMoved: position={}", engine::toString(position_));
    }

    EVENT_TYPE(MouseMoved);
};

class MouseScrolled : public MouseEvent
{
public:
    MouseScrolled(const engine::Vec2f& position, const engine::core::MouseWheel wheel, const float offset)
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
        return fmt::format("MouseScrolled: position={}, wheel={}, offset={}", engine::toString(position_), core::toString(wheel_), offset_);
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
    MouseButtonEvent(const engine::Vec2f& position, const engine::core::MouseButton button)
    : MouseEvent(position)
    , button_(button)
    {}

    engine::core::MouseButton button_;
};

class MouseButtonPressed : public MouseButtonEvent
{
public: 
    MouseButtonPressed(const engine::Vec2f& position, const engine::core::MouseButton button)
    : MouseButtonEvent(position, button)
    {}

    EVENT_TYPE(MouseButtonPressed);
};

class MouseButtonReleased : public MouseButtonEvent
{
public: 
    MouseButtonReleased(const engine::Vec2f& position, const engine::core::MouseButton button)
    : MouseButtonEvent(position, button)
    {}

    EVENT_TYPE(MouseButtonReleased);
};

}
