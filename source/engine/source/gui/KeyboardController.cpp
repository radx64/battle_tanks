#include "engine/gui/KeyboardController.hpp"

#include "engine/gui/Debug.hpp"
#include "engine/gui/EventReceiver.hpp"

namespace engine::gui
{
KeyboardController::KeyboardController(engine::gui::EventReceiver* receiver)
: receiver_(receiver)
{}

void KeyboardController::onKeyPressed(const sf::Event::KeyEvent& keyEvent)
{
    receiver_->receive(engine::gui::event::KeyboardKeyPressed{
        .key = keyEvent.code,
        .modifiers = {
            .alt = keyEvent.alt,
            .shift = keyEvent.shift,
            .control = keyEvent.control,
            .system = keyEvent.system,
        }});

    if (keyEvent.code == engine::gui::event::Key::Tab)
    {
        if (keyEvent.shift)
        {
            receiver_->receive(engine::gui::event::FocusChange{
                .type = engine::gui::event::FocusChange::Type::Previous});
        }
        else
        {
            receiver_->receive(engine::gui::event::FocusChange{
                .type = engine::gui::event::FocusChange::Type::Next});
        }
    }
}

void KeyboardController::onKeyReleased(const sf::Event::KeyEvent& keyEvent)
{
    if (keyEvent.code == engine::gui::event::Key::F12)
    {
        engine::gui::debug::toggle();
    } 

    receiver_->receive(engine::gui::event::KeyboardKeyReleased{
        .key = keyEvent.code,
        .modifiers = {
            .alt = keyEvent.alt,
            .shift = keyEvent.shift,
            .control = keyEvent.control,
            .system = keyEvent.system,
        }});
}

}  // namespace engine::gui
