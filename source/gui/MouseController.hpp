#pragma once
#include <SFML/Graphics.hpp>

#include "engine/input/MouseReceiver.hpp"

namespace gui {class EventReceiver;}

namespace gui
{   

class MouseController : public engine::input::MouseReceiver
{
public:
    MouseController(gui::EventReceiver* receiver, sf::RenderWindow& window, const sf::View& view);

protected:
    gui::EventStatus onButtonPressed(const sf::Vector2f& mousePostion, const sf::Mouse::Button& button, bool doubleClick) override;
    gui::EventStatus onButtonReleased(const sf::Vector2f& mousePostion, const sf::Mouse::Button& button) override;
    gui::EventStatus onMouseMoved(const sf::Vector2f& mousePostion) override;

    sf::Vector2f mapPixelToCoords(const sf::Vector2f& mousePosition);

    gui::EventReceiver* receiver_;
    sf::RenderWindow& window_;
    const sf::View& view_;
};

}  // namespace gui
