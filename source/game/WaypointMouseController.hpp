#pragma once

#include <SFML/Graphics.hpp>

#include "engine/input/MouseReceiver.hpp"

namespace gui {class WindowManager;}

namespace game
{

class WaypointMouseController : public engine::input::MouseReceiver
{
public:
    WaypointMouseController(gui::WindowManager* window_manager, std::vector<sf::Vector2i>& waypoints, sf::RenderWindow& window, sf::View& view);

protected:
    gui::EventStatus onButtonPressed(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button) override;
    gui::EventStatus onButtonReleased(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button) override;
    gui::EventStatus onMouseMoved(const sf::Vector2f& mouse_postion) override;

    sf::Vector2f mapPixelToCoords(const sf::Vector2f& mouse_position);

    gui::WindowManager* windowManager_;
    std::vector<sf::Vector2i>& waypoints_;
    sf::RenderWindow& window_;
    sf::View& view_;

};


}  // namespace game
