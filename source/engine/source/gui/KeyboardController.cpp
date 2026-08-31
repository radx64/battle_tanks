#include "engine/gui/KeyboardController.hpp"

#include "engine/gui/Debug.hpp"
#include "engine/gui/EventReceiver.hpp"

namespace engine::gui
{
KeyboardController::KeyboardController(engine::gui::EventReceiver* receiver)
: receiver_(receiver)
{}

void KeyboardController::onKeyPressed(const engine::events::KeyPressedEvent& keyEvent)
{
    receiver_->receive(keyEvent);

    if (keyEvent.getKeyCode() == engine::core::KeyCode::Tab)
    {
        if (keyEvent.getModifiers().shift)
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

void KeyboardController::onKeyReleased(const engine::events::KeyReleasedEvent& keyEvent)
{
    if (keyEvent.getKeyCode() == engine::core::KeyCode::F12)
    {
        engine::gui::debug::toggle();
    } 

    receiver_->receive(keyEvent);
}

}  // namespace engine::gui
