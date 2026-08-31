#pragma once

#include "engine/events/KeyEvent.hpp"
#include "engine/gui/Event.hpp"


namespace engine::gui
{

enum class EventStatus
{
    Consumed,
    NotConsumed
};

const std::string_view toString(const EventStatus);

class EventReceiver
{
public:
    virtual EventStatus receive(const event::MouseMoved& mouseMovedEvent);
    virtual EventStatus receive(const event::MouseButtonPressed& mouseButtonPressedEvent);
    virtual EventStatus receive(const event::MouseButtonDoublePressed& mouseButtonDoublePressedEvent);
    virtual EventStatus receive(const event::MouseButtonReleased& mouseButtonReleasedEvent);
    virtual EventStatus receive(const event::MouseEntered& mouseEnteredEvent);
    virtual EventStatus receive(const event::MouseLeft& mouseLeftEvent);

    virtual EventStatus receive(const engine::events::KeyPressedEvent& keyboardKeyPressed);
    virtual EventStatus receive(const engine::events::KeyReleasedEvent& keyboardKeyReleased);
    virtual EventStatus receive(const engine::events::TextEnteredEvent& textEntered);

    virtual EventStatus receive(const event::FocusChange& focusChangeEvent);
    virtual EventStatus receive(const event::FocusLost& focusLost);
    virtual EventStatus receive(const event::FocusGained& focusGained);
};

}  // namespace engine::gui
