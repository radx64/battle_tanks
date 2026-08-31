#pragma once

#include <cstdint>

#include "engine/events/Event.hpp"
#include "engine/core/KeyCode.hpp"

namespace engine::events
{

struct KeyModifiers
{
    bool alt;
    bool shift;
    bool control;
    bool system;
};

class KeyEvent : public Event
{
public:
    core::KeyCode getKeyCode() const;
    KeyModifiers getModifiers() const;

    EVENT_CATEGORY(EventCategory::Keyboard | EventCategory::Input);

protected:
    KeyEvent(const core::KeyCode key_code, const KeyModifiers& modifiers);

    core::KeyCode key_code_;
    KeyModifiers modifiers_;
};

class KeyPressedEvent : public KeyEvent
{
public:
    KeyPressedEvent(const core::KeyCode key_code, const KeyModifiers& modifiers);

    std::string toString() const override;

    EVENT_TYPE(KeyPressed);
};

class KeyReleasedEvent : public KeyEvent
{
public:
    KeyReleasedEvent(const core::KeyCode key_code, const KeyModifiers& modifiers);

    std::string toString() const override;

    EVENT_TYPE(KeyReleased);
};

class TextEnteredEvent : public Event
{
public:
    TextEnteredEvent(const uint32_t unicode);

    uint32_t getUnicode() const;

    std::string toString() const override;

    EVENT_TYPE(TextEntered);
    EVENT_CATEGORY(EventCategory::Keyboard | EventCategory::Input);

private:
    uint32_t unicode_;
};

}  // namespace engine::events
