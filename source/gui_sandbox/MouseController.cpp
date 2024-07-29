#include "MouseController.hpp"

#include "gui/WindowManager.hpp"

namespace gui_sandbox
{

MouseController::MouseController(gui::WindowManager* window_manager, sf::RenderWindow& window, const sf::View& view)
: window_manager_(window_manager)
, window_(window)
, view_(view)
{}    

void MouseController::onButtonPressed(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button)
{
    const auto screen_coords = mapPixelToCoords(mouse_postion);

    if (button == sf::Mouse::Button::Left)
    {
        window_manager_->receive(
            gui::event::MouseButtonPressed{
                .button = gui::event::MouseButton::Left,
                .position = {
                    .x = screen_coords.x,
                    .y = screen_coords.y}
            });
    }
}

void MouseController::onButtonReleased(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button)
{
    const auto screen_coords = mapPixelToCoords(mouse_postion);

    if (button == sf::Mouse::Button::Left)
    {
        window_manager_->receive(
            gui::event::MouseButtonReleased{
                .button = gui::event::MouseButton::Left,
                .position = {
                    .x = screen_coords.x,
                    .y = screen_coords.y}
            });
    }
}

void MouseController::onMouseMoved(const sf::Vector2f& mouse_postion)
{   
    const auto screen_coords = mapPixelToCoords(mouse_postion);

    window_manager_->receive(
        gui::event::MouseMoved{.position = gui::event::MousePosition{
            .x = screen_coords.x, 
            .y = screen_coords.y
        }});
}

sf::Vector2f MouseController::mapPixelToCoords(const sf::Vector2f& mouse_position)
{
    return window_.mapPixelToCoords(sf::Vector2i{(int)mouse_position.x, (int)mouse_position.y}, view_);
}


}  // namespace gui_sandbox
