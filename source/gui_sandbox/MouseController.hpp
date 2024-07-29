#pragma once


#include <SFML/Graphics.hpp>

#include "engine/input/MouseReceiver.hpp"

namespace gui {class WindowManager;}

namespace gui_sandbox
{   

// TODO: Rewrite original MouseController to be expandable (eg game uses waypoints)
// To not copy this code everywhere 
class MouseController : public engine::input::MouseReceiver
{
public:
    MouseController(gui::WindowManager* window_manager, sf::RenderWindow& window, const sf::View& view);

protected:
    void onButtonPressed(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button) override;
    void onButtonReleased(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button) override;
    void onMouseMoved(const sf::Vector2f& mouse_postion) override;

    sf::Vector2f mapPixelToCoords(const sf::Vector2f& mouse_position);

    gui::WindowManager* window_manager_;
    sf::RenderWindow& window_;
    const sf::View& view_;
};


}  // namespace gui_sandbox


