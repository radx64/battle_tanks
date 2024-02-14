#include "game/MouseController.hpp"

#include "gui/WindowManager.hpp"

namespace game
{

MouseController::MouseController(gui::WindowManager* window_manager, std::vector<sf::Vector2i>& waypoints, sf::RenderWindow& window, sf::View& view)
: window_manager_(window_manager)
, waypoints_(waypoints)
, window_(window)
, view_(view)
{}    

void MouseController::onButtonPressed(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button)
{
    if (button == sf::Mouse::Button::Left)
    {
        auto result = window_manager_->receive(
            gui::event::MouseButtonPressed{
                .button = gui::event::MouseButton::Left,
                .position = {
                    .x = mouse_postion.x,
                    .y = mouse_postion.y}
            });

        if (result == gui::EventStatus::NotConsumed)
        {
            auto waypoint_position  = window_.mapPixelToCoords(sf::Vector2i{(int)mouse_postion.x, (int)mouse_postion.y}, view_);
            waypoints_.push_back(sf::Vector2i{waypoint_position});
        }

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

}  // namespace game
