#pragma once

#include "engine/events/KeyEvent.hpp"
#include "engine/gui/Events.hpp"


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
    virtual EventStatus receive(const engine::events::MouseMoved& mouseMovedEvent);
    virtual EventStatus receive(const engine::events::MouseButtonPressed& mouseButtonPressedEvent);
    virtual EventStatus receive(const engine::gui::events::MouseButtonDoublePressed& mouseButtonDoublePressedEvent);
    virtual EventStatus receive(const engine::events::MouseButtonReleased& mouseButtonReleasedEvent);
    virtual EventStatus receive(const engine::gui::events::MouseEntered& mouseEnteredEvent);
    virtual EventStatus receive(const engine::gui::events::MouseLeft& mouseLeftEvent);
    virtual EventStatus receive(const engine::events::MouseScrolled& mouseScrolledEvent);

    virtual EventStatus receive(const engine::events::KeyPressed& keyboardKeyPressed);
    virtual EventStatus receive(const engine::events::KeyReleased& keyboardKeyReleased);
    virtual EventStatus receive(const engine::events::TextEntered& textEntered);

    virtual EventStatus receive(const engine::gui::events::FocusChange& focusChangeEvent);
    virtual EventStatus receive(const engine::gui::events::FocusLost& focusLost);
    virtual EventStatus receive(const engine::gui::events::FocusGained& focusGained);
};

}  // namespace engine::gui
