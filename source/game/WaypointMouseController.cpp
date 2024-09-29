#include "game/WaypointMouseController.hpp"

#include "gui/WindowManager.hpp"

namespace game
{

WaypointMouseController::WaypointMouseController(gui::WindowManager* window_manager, std::vector<sf::Vector2i>& waypoints, sf::RenderWindow& window, sf::View& view)
: windowManager_(window_manager)
, waypoints_(waypoints)
, window_(window)
, view_(view)
{}    

gui::EventStatus WaypointMouseController::onButtonPressed(const sf::Vector2f& mouse_position, const sf::Mouse::Button& button)
{
    const auto screenCoords = mapPixelToCoords(mouse_position);

    if (button != sf::Mouse::Button::Left)
    {
        return gui::EventStatus::NotConsumed;
    }

    auto waypoint_position  = screenCoords;
    waypoints_.push_back(sf::Vector2i{waypoint_position});
    return gui::EventStatus::Consumed;
}

gui::EventStatus WaypointMouseController::onButtonReleased(const sf::Vector2f& mouse_position, const sf::Mouse::Button& button)
{
    UNUSED2(mouse_position, button);
    return gui::EventStatus::NotConsumed;
}

gui::EventStatus WaypointMouseController::onMouseMoved(const sf::Vector2f& mouse_position)
{
    UNUSED(mouse_position);
    return gui::EventStatus::NotConsumed;
}

sf::Vector2f WaypointMouseController::mapPixelToCoords(const sf::Vector2f& mouse_position)
{
    return window_.mapPixelToCoords(sf::Vector2i{(int)mouse_position.x, (int)mouse_position.y}, view_);
}

}  // namespace game
