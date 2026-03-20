#include "gui/KeyboardController.hpp"

#include "gui/Debug.hpp"
#include "gui/EventReceiver.hpp"

namespace gui
{
KeyboardController::KeyboardController(gui::EventReceiver* receiver)
: receiver_(receiver)
{}

void KeyboardController::onKeyPressed(const sf::Event::KeyEvent& keyEvent)
{
    receiver_->receive(gui::event::KeyboardKeyPressed{
        .key = keyEvent.code,
        .modifiers = {
            .alt = keyEvent.alt,
            .shift = keyEvent.shift,
            .control = keyEvent.control,
            .system = keyEvent.system,
        }});

    if (keyEvent.code == gui::event::Key::Tab)
    {
        if (keyEvent.shift)
        {
            receiver_->receive(gui::event::FocusChange{
                .type = gui::event::FocusChange::Type::Previous});
        }
        else
        {
            receiver_->receive(gui::event::FocusChange{
                .type = gui::event::FocusChange::Type::Next});
        }
    }
}

void KeyboardController::onKeyReleased(const sf::Event::KeyEvent& keyEvent)
{
    if (keyEvent.code == gui::event::Key::F12)
    {
        gui::debug::toggle();
    } 

    receiver_->receive(gui::event::KeyboardKeyReleased{
        .key = keyEvent.code,
        .modifiers = {
            .alt = keyEvent.alt,
            .shift = keyEvent.shift,
            .control = keyEvent.control,
            .system = keyEvent.system,
        }});
}

}  // namespace gui
