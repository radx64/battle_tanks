#include "engine/gui/EventReceiver.hpp"

namespace engine::gui
{

const std::string_view toString(const EventStatus eventStatus)
{
    switch (eventStatus)
    {
        case EventStatus::Consumed : return "Consumed";
        case EventStatus::NotConsumed : return "NotConsumed";
        default : return "Unknown status";
    }
}

#define EMPTY_RECEIVE_METHOD(Class, Event)\
    EventStatus Class::receive(const Event&)\
    { return EventStatus::NotConsumed; }

EMPTY_RECEIVE_METHOD(EventReceiver, event::MouseMoved);
EMPTY_RECEIVE_METHOD(EventReceiver, event::MouseButtonPressed);
EMPTY_RECEIVE_METHOD(EventReceiver, event::MouseButtonDoublePressed);
EMPTY_RECEIVE_METHOD(EventReceiver, event::MouseButtonReleased);
EMPTY_RECEIVE_METHOD(EventReceiver, event::MouseEntered);
EMPTY_RECEIVE_METHOD(EventReceiver, event::MouseLeft);

EMPTY_RECEIVE_METHOD(EventReceiver, engine::events::KeyPressedEvent);
EMPTY_RECEIVE_METHOD(EventReceiver, engine::events::KeyReleasedEvent);
EMPTY_RECEIVE_METHOD(EventReceiver, engine::events::TextEnteredEvent);

EMPTY_RECEIVE_METHOD(EventReceiver, event::FocusChange);
EMPTY_RECEIVE_METHOD(EventReceiver, event::FocusLost);
EMPTY_RECEIVE_METHOD(EventReceiver, event::FocusGained);

#undef EMPTY_ON_METHOD

}  // namespace engine::gui
