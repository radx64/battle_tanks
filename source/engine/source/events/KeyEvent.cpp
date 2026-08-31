#include "engine/events/KeyEvent.hpp"

#include <fmt/format.h>

namespace engine::events
{

namespace keymodifiers
{
std::string toString(const KeyModifiers modifiers)
{
    return fmt::format("alt={}, shift={}, control={}, system={}", modifiers.alt, modifiers.shift, modifiers.control, modifiers.system);
}
}   // namespace keymodifiers


core::KeyCode KeyEvent::getKeyCode() const
{
    return key_code_;
}

KeyModifiers KeyEvent::getModifiers() const
{
    return modifiers_;
}

KeyEvent::KeyEvent(const core::KeyCode key_code, const KeyModifiers& modifiers)
: key_code_(key_code), modifiers_(modifiers)
{}

KeyPressedEvent::KeyPressedEvent(const core::KeyCode key_code, const KeyModifiers& modifiers)
: KeyEvent(key_code, modifiers)
{}

std::string KeyPressedEvent::toString() const
{
    return fmt::format("KeyPressedEvent: {} {}", static_cast<int>(key_code_), keymodifiers::toString(modifiers_));
}

KeyReleasedEvent::KeyReleasedEvent(const core::KeyCode key_code, const KeyModifiers& modifiers)
: KeyEvent(key_code, modifiers)
{}

std::string KeyReleasedEvent::toString() const
{
    return fmt::format("KeyReleasedEvent: {}", static_cast<int>(key_code_), keymodifiers::toString(modifiers_));
}

TextEnteredEvent::TextEnteredEvent(const uint32_t unicode)
: unicode_(unicode)
{}

uint32_t TextEnteredEvent::getUnicode() const
{
    return unicode_;
}

std::string TextEnteredEvent::toString() const
{
    return fmt::format("TextEnteredEvent: {}", unicode_);
}

}//  namespace engine::events
