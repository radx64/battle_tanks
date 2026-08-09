#include "engine/gui/MouseController.hpp"

#include "engine/gui/EventReceiver.hpp"

namespace engine::gui
{

MouseController::MouseController(engine::gui::EventReceiver* receiver, sf::RenderWindow& window, const sf::View& view)
: receiver_(receiver)
, window_(window)
, view_(view)
{}    

engine::gui::EventStatus MouseController::onButtonPressed(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& sfmlButton, bool double_click)
{
    const auto screen_coords = mapPixelToCoords(mouse_postion);

    const auto guiButton = [&]() {
        switch (sfmlButton)
        {
            case sf::Mouse::Button::Left:   return engine::gui::event::MouseButton::Left;
            case sf::Mouse::Button::Right:  return engine::gui::event::MouseButton::Right;
            case sf::Mouse::Button::Middle: return engine::gui::event::MouseButton::Middle;
            case sf::Mouse::Button::XButton1: return engine::gui::event::MouseButton::XButton1;
            case sf::Mouse::Button::XButton2: return engine::gui::event::MouseButton::XButton2;
            default: return engine::gui::event::MouseButton::Left;
        }
    }();

    if (double_click)
    {
        return receiver_->receive(
            engine::gui::event::MouseButtonDoublePressed{
                .button = guiButton,
                .position = {.x = screen_coords.x, .y = screen_coords.y}
            });
    }

    return receiver_->receive(
        engine::gui::event::MouseButtonPressed{
            .button = guiButton,
            .position = {.x = screen_coords.x, .y = screen_coords.y}
        });
}

engine::gui::EventStatus MouseController::onButtonReleased(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& sfmlButton)
{
    const auto screen_coords = mapPixelToCoords(mouse_postion);

    const auto guiButton = [&]() {
        switch (sfmlButton)
        {
            case sf::Mouse::Button::Left:   return engine::gui::event::MouseButton::Left;
            case sf::Mouse::Button::Right:  return engine::gui::event::MouseButton::Right;
            case sf::Mouse::Button::Middle: return engine::gui::event::MouseButton::Middle;
            case sf::Mouse::Button::XButton1: return engine::gui::event::MouseButton::XButton1;
            case sf::Mouse::Button::XButton2: return engine::gui::event::MouseButton::XButton2;
            default: return engine::gui::event::MouseButton::Left;
        }
    }();

    return receiver_->receive(
        engine::gui::event::MouseButtonReleased{
            .button = guiButton,
            .position = {.x = screen_coords.x, .y = screen_coords.y}
        });
}

engine::gui::EventStatus MouseController::onMouseMoved(const sf::Vector2f& mouse_postion)
{   
    const auto screen_coords = mapPixelToCoords(mouse_postion);

    const auto result = receiver_->receive(
        engine::gui::event::MouseMoved{.position = engine::gui::event::MousePosition{
            .x = screen_coords.x, 
            .y = screen_coords.y
        }});
    return result;
}

sf::Vector2f MouseController::mapPixelToCoords(const sf::Vector2f& mouse_position)
{
    return window_.mapPixelToCoords(sf::Vector2i{
        static_cast<int>(mouse_position.x), 
        static_cast<int>(mouse_position.y)}, 
        view_);
}

}  // namespace engine::gui
