#include "gui/MouseController.hpp"

#include "gui/WindowManager.hpp"

namespace gui
{

MouseController::MouseController(gui::WindowManager* windowManager, sf::RenderWindow& window, const sf::View& view)
: windowManager_(windowManager)
, window_(window)
, view_(view)
{}    

gui::EventStatus MouseController::onButtonPressed(const sf::Vector2f& mousePostion, const sf::Mouse::Button& button)
{
    const auto screenCoords = mapPixelToCoords(mousePostion);

    if (button != sf::Mouse::Button::Left)
    {
        return gui::EventStatus::NotConsumed;
    }

    const auto result = windowManager_->receive(
        gui::event::MouseButtonPressed{
            .button = gui::event::MouseButton::Left,
            .position = {
                .x = screenCoords.x,
                .y = screenCoords.y}
        });

    return result; 
}

gui::EventStatus MouseController::onButtonReleased(const sf::Vector2f& mousePostion, const sf::Mouse::Button& button)
{
    const auto screenCoords = mapPixelToCoords(mousePostion);

    if (button != sf::Mouse::Button::Left)
    {
        return gui::EventStatus::NotConsumed;
    }

    const auto result = windowManager_->receive(
        gui::event::MouseButtonReleased{
            .button = gui::event::MouseButton::Left,
            .position = {
                .x = screenCoords.x,
                .y = screenCoords.y}
        });
    return result;
}

gui::EventStatus MouseController::onMouseMoved(const sf::Vector2f& mousePostion)
{   
    const auto screenCoords = mapPixelToCoords(mousePostion);

    const auto result = windowManager_->receive(
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
