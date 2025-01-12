#include "gui/KeyboardController.hpp"

#include "gui/Debug.hpp"
#include "gui/WindowManager.hpp"

namespace gui
{
KeyboardController::KeyboardController(gui::WindowManager* windowManager)
: windowManager_(windowManager)
{}

void KeyboardController::onKeyPressed(const sf::Event::KeyEvent& keyEvent)
{
    const auto status = windowManager_->receive(gui::event::KeyboardKeyPressed{
        .key = keyEvent.code,
        .modifiers = {
            .alt = keyEvent.alt,
            .shift = keyEvent.shift,
            .control = keyEvent.control,
            .system = keyEvent.system,
        }});

    if (status == gui::EventStatus::NotConsumed and keyEvent.code == gui::event::Key::Tab)
    {
        if (keyEvent.shift)
        {
            windowManager_->receive(gui::event::FocusChange{
                .type = gui::event::FocusChange::Type::Previous});
        }
        else
        {
            windowManager_->receive(gui::event::FocusChange{
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

    windowManager_->receive(gui::event::KeyboardKeyReleased{
        .key = keyEvent.code,
        .modifiers = {
            .alt = keyEvent.alt,
            .shift = keyEvent.shift,
            .control = keyEvent.control,
            .system = keyEvent.system,
        }});
}

}  // namespace gui
