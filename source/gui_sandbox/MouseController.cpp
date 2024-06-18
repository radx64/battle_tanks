#include "MouseController.hpp"

#include "gui/WindowManager.hpp"

namespace gui_sandbox
{

MouseController::MouseController(gui::WindowManager* window_manager)
: window_manager_(window_manager)
{}    

void MouseController::onButtonPressed(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button)
{
    if (button == sf::Mouse::Button::Left)
    {
        window_manager_->receive(
            gui::event::MouseButtonPressed{
                .button = gui::event::MouseButton::Left,
                .position = {
                    .x = mouse_postion.x,
                    .y = mouse_postion.y}
            });
    }
}

void MouseController::onButtonReleased(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button)
{
    if (button == sf::Mouse::Button::Left)
    {
        window_manager_->receive(
            gui::event::MouseButtonReleased{
                .button = gui::event::MouseButton::Left,
                .position = {
                    .x = mouse_postion.x,
                    .y = mouse_postion.y}
            });
    }
}

void MouseController::onMouseMoved(const sf::Vector2f& mouse_postion)
{
    window_manager_->receive(
        gui::event::MouseMoved{.position = gui::event::MousePosition{
            .x = mouse_postion.x, 
            .y = mouse_postion.y
        }});
}

}  // namespace gui_sandbox
