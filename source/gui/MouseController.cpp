#include "gui/MouseController.hpp"

#include "gui/EventReceiver.hpp"

namespace gui
{

MouseController::MouseController(gui::EventReceiver* receiver, sf::RenderWindow& window, const sf::View& view)
: receiver_(receiver)
, window_(window)
, view_(view)
{}    

gui::EventStatus MouseController::onButtonPressed(const sf::Vector2f& mousePostion, const sf::Mouse::Button& sfmlButton, bool doubleClick)
{
    const auto screenCoords = mapPixelToCoords(mousePostion);

    const auto guiButton = [&]() {
        switch (sfmlButton)
        {
            case sf::Mouse::Button::Left:   return gui::event::MouseButton::Left;
            case sf::Mouse::Button::Right:  return gui::event::MouseButton::Right;
            case sf::Mouse::Button::Middle: return gui::event::MouseButton::Middle;
            case sf::Mouse::Button::XButton1: return gui::event::MouseButton::XButton1;
            case sf::Mouse::Button::XButton2: return gui::event::MouseButton::XButton2;
            default: return gui::event::MouseButton::Left;
        }
    }();

    if (doubleClick)
    {
        return receiver_->receive(
            gui::event::MouseButtonDoublePressed{
                .button = guiButton,
                .position = {.x = screenCoords.x, .y = screenCoords.y}
            });
    }

    return receiver_->receive(
        gui::event::MouseButtonPressed{
            .button = guiButton,
            .position = {.x = screenCoords.x, .y = screenCoords.y}
        });
}

gui::EventStatus MouseController::onButtonReleased(const sf::Vector2f& mousePostion, const sf::Mouse::Button& sfmlButton)
{
    const auto screenCoords = mapPixelToCoords(mousePostion);

    const auto guiButton = [&]() {
        switch (sfmlButton)
        {
            case sf::Mouse::Button::Left:   return gui::event::MouseButton::Left;
            case sf::Mouse::Button::Right:  return gui::event::MouseButton::Right;
            case sf::Mouse::Button::Middle: return gui::event::MouseButton::Middle;
            case sf::Mouse::Button::XButton1: return gui::event::MouseButton::XButton1;
            case sf::Mouse::Button::XButton2: return gui::event::MouseButton::XButton2;
            default: return gui::event::MouseButton::Left;
        }
    }();

    return receiver_->receive(
        gui::event::MouseButtonReleased{
            .button = guiButton,
            .position = {.x = screenCoords.x, .y = screenCoords.y}
        });
}

gui::EventStatus MouseController::onMouseMoved(const sf::Vector2f& mousePostion)
{   
    const auto screenCoords = mapPixelToCoords(mousePostion);

    const auto result = receiver_->receive(
        gui::event::MouseMoved{.position = gui::event::MousePosition{
            .x = screenCoords.x, 
            .y = screenCoords.y
        }});
    return result;
}

sf::Vector2f MouseController::mapPixelToCoords(const sf::Vector2f& mousePosition)
{
    return window_.mapPixelToCoords(sf::Vector2i{
        static_cast<int>(mousePosition.x), 
        static_cast<int>(mousePosition.y)}, 
        view_);
}

}  // namespace gui
