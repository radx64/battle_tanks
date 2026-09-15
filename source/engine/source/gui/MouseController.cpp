#include "engine/gui/MouseController.hpp"

#include "engine/gui/Events.hpp"
#include "engine/gui/EventReceiver.hpp"

#include "engine/temporary/ToSfFromSf.hpp"

namespace engine::gui
{

MouseController::MouseController(engine::gui::EventReceiver* receiver, sf::RenderWindow& window, const sf::View& view)
: receiver_(receiver)
, window_(window)
, view_(view)
{}    

engine::gui::EventStatus MouseController::onButtonPressed(const engine::Vec2f& mouse_postion, const engine::core::MouseButton& button, bool double_click)
{
    const auto screen_coords = mapPixelToCoords(mouse_postion);

    if (double_click)
    {
        return receiver_->receive(engine::gui::events::MouseButtonDoublePressed{mouse_postion, button});
    }

    return receiver_->receive(
        engine::events::MouseButtonPressed{engine::Vec2f{screen_coords.x, screen_coords.y}, button});
}

engine::gui::EventStatus MouseController::onButtonReleased(const engine::Vec2f& mouse_postion, const engine::core::MouseButton& button)
{
    const auto screen_coords = mapPixelToCoords(mouse_postion);

    return receiver_->receive(
        engine::events::MouseButtonReleased{engine::Vec2f{screen_coords.x, screen_coords.y}, button});
}

engine::gui::EventStatus MouseController::onMouseMoved(const engine::Vec2f& mouse_postion)
{   
    const auto screen_coords = mapPixelToCoords(mouse_postion);

    const auto result = receiver_->receive(
        engine::events::MouseMoved{engine::Vec2f{screen_coords.x, screen_coords.y}});
    return result;
}
engine::gui::EventStatus MouseController::onMouseScrolled(const engine::Vec2f& mouse_postion, const engine::core::MouseWheel& wheel, const float delta)
{
    const auto screen_coords = mapPixelToCoords(mouse_postion);

    const auto result = receiver_->receive(
        engine::events::MouseScrolled{engine::Vec2f{screen_coords.x, screen_coords.y}, wheel, delta});
    return result;
}

sf::Vector2f MouseController::mapPixelToCoords(const engine::Vec2f& mouse_position)
{
    return window_.mapPixelToCoords(sf::Vector2i{
        static_cast<int>(mouse_position.x), 
        static_cast<int>(mouse_position.y)}, 
        view_);
}

}  // namespace engine::gui
