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
    gui::EventStatus onButtonPressed(const sf::Vector2f& mousePostion, const sf::Mouse::Button& button, bool doubleClick) override;
    gui::EventStatus onButtonReleased(const sf::Vector2f& mousePostion, const sf::Mouse::Button& button) override;
    gui::EventStatus onMouseMoved(const sf::Vector2f& mousePostion) override;

    sf::Vector2f mapPixelToCoords(const sf::Vector2f& mousePosition);

    std::vector<sf::Vector2i>& waypoints_;
    sf::RenderWindow& window_;
    sf::View& view_;
};


}  // namespace game
