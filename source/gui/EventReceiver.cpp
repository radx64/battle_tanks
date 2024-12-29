#include "gui/EventReceiver.hpp"

namespace gui
{

#define EMPTY_RECEIVE_METHOD(Class, Event)\
    EventStatus Class::receive(const Event&)\
    { return EventStatus::NotConsumed; }

EMPTY_RECEIVE_METHOD(EventReceiver, event::MouseMoved);
EMPTY_RECEIVE_METHOD(EventReceiver, event::MouseButtonPressed);
EMPTY_RECEIVE_METHOD(EventReceiver, event::MouseButtonDoublePressed);
EMPTY_RECEIVE_METHOD(EventReceiver, event::MouseButtonReleased);
EMPTY_RECEIVE_METHOD(EventReceiver, event::MouseEntered);
EMPTY_RECEIVE_METHOD(EventReceiver, event::MouseLeft);

EMPTY_RECEIVE_METHOD(EventReceiver, event::KeyboardKeyPressed);
EMPTY_RECEIVE_METHOD(EventReceiver, event::KeyboardKeyReleased);
EMPTY_RECEIVE_METHOD(EventReceiver, event::TextEntered);

EMPTY_RECEIVE_METHOD(EventReceiver, event::FocusChange);
EMPTY_RECEIVE_METHOD(EventReceiver, event::FocusLost);
EMPTY_RECEIVE_METHOD(EventReceiver, event::FocusGained);

#undef EMPTY_ON_METHOD

}  // namespace gui
