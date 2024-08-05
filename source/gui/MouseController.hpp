#pragma once
#include <SFML/Graphics.hpp>

#include "engine/input/MouseReceiver.hpp"

namespace gui {class WindowManager;}

namespace gui
{   

class MouseController : public engine::input::MouseReceiver
{
public:
    MouseController(gui::WindowManager* window_manager, sf::RenderWindow& window, const sf::View& view);

protected:
    gui::EventStatus onButtonPressed(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button) override;
    gui::EventStatus onButtonReleased(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button) override;
    gui::EventStatus onMouseMoved(const sf::Vector2f& mouse_postion) override;

    sf::Vector2f mapPixelToCoords(const sf::Vector2f& mouse_position);

    gui::WindowManager* window_manager_;
    sf::RenderWindow& window_;
    const sf::View& view_;
};

}  // namespace gui
