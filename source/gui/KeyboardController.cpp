#include "gui/KeyboardController.hpp"

#include "gui/WindowManager.hpp"

namespace gui
{
KeyboardController::KeyboardController(gui::WindowManager* window_manager)
: window_manager_(window_manager)
{}

void KeyboardController::onKeyPressed(const sf::Event::KeyEvent& keyEvent)
{
    window_manager_->receive(gui::event::KeyboardKeyPressed{
        .key = keyEvent.code,
        .modifiers = {
            .alt = keyEvent.alt,
            .shift = keyEvent.shift,
            .control = keyEvent.control,
            .system = keyEvent.system,
        }});  
}

void KeyboardController::onKeyReleased(const sf::Event::KeyEvent& keyEvent)
{
    window_manager_->receive(gui::event::KeyboardKeyReleased{
        .key = keyEvent.code, 
        .modifiers = {
            .alt = keyEvent.alt,
            .shift = keyEvent.shift,
            .control = keyEvent.control,
            .system = keyEvent.system,
        }});
}

}  // namespace gui
