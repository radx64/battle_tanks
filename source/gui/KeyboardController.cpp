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
            engine::Option(keyEvent.alt, gui::event::KeyboardModifier::Alt),
            engine::Option(keyEvent.shift, gui::event::KeyboardModifier::Shift),
            engine::Option(keyEvent.control, gui::event::KeyboardModifier::Control),
            engine::Option(keyEvent.system, gui::event::KeyboardModifier::System)
        }});  
}

void KeyboardController::onKeyReleased(const sf::Event::KeyEvent& keyEvent)
{
    window_manager_->receive(gui::event::KeyboardKeyReleased{
        .key = keyEvent.code, 
        .modifiers = {
            engine::Option(keyEvent.alt, gui::event::KeyboardModifier::Alt),
            engine::Option(keyEvent.shift, gui::event::KeyboardModifier::Shift),
            engine::Option(keyEvent.control, gui::event::KeyboardModifier::Control),
            engine::Option(keyEvent.system, gui::event::KeyboardModifier::System)
        }});
}

}  // namespace gui
