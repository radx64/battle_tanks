#pragma once

#include <SFML/Graphics.hpp>

#include "engine/input/MouseReceiver.hpp"

namespace game
{

class WaypointMouseController : public engine::input::MouseReceiver
{
public:
    WaypointMouseController(std::vector<sf::Vector2i>& waypoints, sf::RenderWindow& window, sf::View& view);

protected:
    engine::gui::EventStatus onButtonPressed(const engine::Vec2f& mouse_postion, const engine::core::MouseButton& button, bool double_click) override;
    engine::gui::EventStatus onButtonReleased(const engine::Vec2f& mouse_postion, const engine::core::MouseButton& button) override;
    engine::gui::EventStatus onMouseMoved(const engine::Vec2f& mouse_postion) override;
    engine::gui::EventStatus onMouseScrolled(const engine::Vec2f& mouse_position, const engine::core::MouseWheel& wheel, const float delta) override;

    sf::Vector2f mapPixelToCoords(const engine::Vec2f& mouse_position);

    std::vector<sf::Vector2i>& waypoints_;
    sf::RenderWindow& window_;
    sf::View& view_;
};


}  // namespace game
