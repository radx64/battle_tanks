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

void MouseController::onButtonPressed(const sf::Vector2f& mouse_position, const sf::Mouse::Button& button)
{
    const auto screenCoords = mapPixelToCoords(mouse_position);

    if (button == sf::Mouse::Button::Left)
    {
        auto result = window_manager_->receive(
            gui::event::MouseButtonPressed{
                .button = gui::event::MouseButton::Left,
                .position = {
                    .x = screenCoords.x,
                    .y = screenCoords.y}
            });

        if (result == gui::EventStatus::NotConsumed)
        {
            auto waypoint_position  = screenCoords;
            waypoints_.push_back(sf::Vector2i{waypoint_position});
        }
    }
}

void MouseController::onButtonReleased(const sf::Vector2f& mouse_position, const sf::Mouse::Button& button)
{
    const auto screenCoords = mapPixelToCoords(mouse_position);

    if (button == sf::Mouse::Button::Left)
    {
        window_manager_->receive(
            gui::event::MouseButtonReleased{
                .button = gui::event::MouseButton::Left,
                .position = {
                    .x = screenCoords.x,
                    .y = screenCoords.y}
            });
    }
}

void MouseController::onMouseMoved(const sf::Vector2f& mouse_position)
{
    const auto screenCoords = mapPixelToCoords(mouse_position);

    window_manager_->receive(
        gui::event::MouseMoved{.position = gui::event::MousePosition{
            .x = screenCoords.x, 
            .y = screenCoords.y
        }});
}

sf::Vector2f MouseController::mapPixelToCoords(const sf::Vector2f& mouse_position)
{
    return window_.mapPixelToCoords(sf::Vector2i{(int)mouse_position.x, (int)mouse_position.y}, view_);
}

}  // namespace game
