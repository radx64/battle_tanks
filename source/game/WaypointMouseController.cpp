#include "game/WaypointMouseController.hpp"

#include "gui/WindowManager.hpp"

namespace game
{

WaypointMouseController::WaypointMouseController(gui::WindowManager* windowManager, std::vector<sf::Vector2i>& waypoints, sf::RenderWindow& window, sf::View& view)
: windowManager_(windowManager)
, waypoints_(waypoints)
, window_(window)
, view_(view)
{}    

gui::EventStatus WaypointMouseController::onButtonPressed(const sf::Vector2f& mousePosition, const sf::Mouse::Button& button, bool doubleClick)
{
    UNUSED(doubleClick);

    const auto screenCoords = mapPixelToCoords(mousePosition);

    if (button != sf::Mouse::Button::Left)
    {
        return gui::EventStatus::NotConsumed;
    }

    auto waypointPosition  = screenCoords;
    waypoints_.push_back(sf::Vector2i{waypointPosition});
    return gui::EventStatus::Consumed;
}

gui::EventStatus WaypointMouseController::onButtonReleased(const sf::Vector2f& mousePosition, const sf::Mouse::Button& button)
{
    UNUSED2(mousePosition, button);
    return gui::EventStatus::NotConsumed;
}

gui::EventStatus WaypointMouseController::onMouseMoved(const sf::Vector2f& mousePosition)
{
    UNUSED(mousePosition);
    return gui::EventStatus::NotConsumed;
}

sf::Vector2f WaypointMouseController::mapPixelToCoords(const sf::Vector2f& mousePosition)
{
    return window_.mapPixelToCoords(sf::Vector2i{(int)mousePosition.x, (int)mousePosition.y}, view_);
}

}  // namespace game
