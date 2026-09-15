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

EMPTY_RECEIVE_METHOD(EventReceiver, engine::events::MouseMoved);
EMPTY_RECEIVE_METHOD(EventReceiver, engine::events::MouseButtonPressed);
EMPTY_RECEIVE_METHOD(EventReceiver, engine::gui::events::MouseButtonDoublePressed);
EMPTY_RECEIVE_METHOD(EventReceiver, engine::events::MouseButtonReleased);
EMPTY_RECEIVE_METHOD(EventReceiver, engine::gui::events::MouseEntered);
EMPTY_RECEIVE_METHOD(EventReceiver, engine::gui::events::MouseLeft);
EMPTY_RECEIVE_METHOD(EventReceiver, engine::events::MouseScrolled);

EMPTY_RECEIVE_METHOD(EventReceiver, engine::events::KeyPressed);
EMPTY_RECEIVE_METHOD(EventReceiver, engine::events::KeyReleased);
EMPTY_RECEIVE_METHOD(EventReceiver, engine::events::TextEntered);

EMPTY_RECEIVE_METHOD(EventReceiver, engine::gui::events::FocusChange);
EMPTY_RECEIVE_METHOD(EventReceiver, engine::gui::events::FocusLost);
EMPTY_RECEIVE_METHOD(EventReceiver, engine::gui::events::FocusGained);

#undef EMPTY_ON_METHOD

}  // namespace engine::gui
