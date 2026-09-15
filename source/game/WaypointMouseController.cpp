#include "game/WaypointMouseController.hpp"

#include "engine/Logger.hpp"

namespace game
{

WaypointMouseController::WaypointMouseController(std::vector<sf::Vector2i>& waypoints, sf::RenderWindow& window, sf::View& view)
: waypoints_(waypoints)
, window_(window)
, view_(view)
{}

engine::gui::EventStatus WaypointMouseController::onButtonPressed(const engine::Vec2f& mouse_position, const engine::core::MouseButton& button, bool)
{
    const auto screen_coords = mapPixelToCoords(mouse_position);

    if (button != engine::core::MouseButton::Left)
    {
        return engine::gui::EventStatus::NotConsumed;
    }

    auto waypoint_position  = screen_coords;
    waypoints_.push_back(sf::Vector2i{waypoint_position});
    return engine::gui::EventStatus::Consumed;
}

engine::gui::EventStatus WaypointMouseController::onButtonReleased(const engine::Vec2f&, const engine::core::MouseButton&)
{
    return engine::gui::EventStatus::NotConsumed;
}

engine::gui::EventStatus WaypointMouseController::onMouseMoved(const engine::Vec2f&)
{
    return engine::gui::EventStatus::NotConsumed;
}

engine::gui::EventStatus WaypointMouseController::onMouseScrolled(const engine::Vec2f&, const engine::core::MouseWheel&, const float)
{
    return engine::gui::EventStatus::NotConsumed;
}

sf::Vector2f WaypointMouseController::mapPixelToCoords(const engine::Vec2f& mouse_position)
{
    engine::Logger logger{"WaypointMouseController"};
    logger.vip(fmt::format("Mapping pixel to coords: mouse_position = ({}, {})", mouse_position.x, mouse_position.y));
    return window_.mapPixelToCoords(sf::Vector2i{(int)mouse_position.x, (int)mouse_position.y}, view_);
}

}  // namespace game
