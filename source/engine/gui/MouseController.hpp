#pragma once
#include <SFML/Graphics.hpp>

#include "engine/input/MouseReceiver.hpp"

namespace engine::gui {class EventReceiver;}

namespace engine::gui
{   

class MouseController : public engine::input::MouseReceiver
{
public:
    MouseController(engine::gui::EventReceiver* receiver, sf::RenderWindow& window, const sf::View& view);

protected:
    engine::gui::EventStatus onButtonPressed(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button, bool double_click) override;
    engine::gui::EventStatus onButtonReleased(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button) override;
    engine::gui::EventStatus onMouseMoved(const sf::Vector2f& mouse_postion) override;

    sf::Vector2f mapPixelToCoords(const sf::Vector2f& mouse_position);

    engine::gui::EventReceiver* receiver_;
    sf::RenderWindow& window_;
    const sf::View& view_;
};

}  // namespace engine::gui
