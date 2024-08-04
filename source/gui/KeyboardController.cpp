#include "gui/KeyboardController.hpp"

#include "gui/WindowManager.hpp"

namespace gui
{
KeyboardController::KeyboardController(gui::WindowManager* window_manager)
: window_manager_(window_manager)
{}

void KeyboardController::onKeyPressed(const sf::Keyboard::Key key)
{
    window_manager_->receive(gui::event::KeyboardKeyPressed{.key = key});  
}

void KeyboardController::onKeyReleased(const sf::Keyboard::Key key)
{
    window_manager_->receive(gui::event::KeyboardKeyReleased{.key = key});  
}

}  // namespace gui