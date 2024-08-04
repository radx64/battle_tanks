#include "engine/input/MouseHandler.hpp"
#include "engine/input/MouseReceiver.hpp"

#include <unordered_map>
#include <vector>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace engine::input
{

void MouseHandler::subscribe(MouseReceiver* receiver)
{
    receivers_.push_back(receiver);
    receiver->attach(this);
}

void MouseHandler::unsubscribe(MouseReceiver* receiver)
{
    std::remove(receivers_.begin(), receivers_.end(), receiver);
}

void MouseHandler::handleButtonPressed(const sf::Event::MouseButtonEvent& event)
{
    const auto button = event.button;
    buttons_states_[button] = true;
    for (auto* receiver : receivers_)
    {
        const auto eventStatus = receiver->onButtonPressed(mouse_position_, button);
        if (eventStatus == gui::EventStatus::Consumed) break;
    }
}

void MouseHandler::handleButtonReleased(const sf::Event::MouseButtonEvent& event)
{
    const auto button = event.button;
    buttons_states_[button] = false;
    for (auto* receiver : receivers_)
    {
        const auto eventStatus = receiver->onButtonReleased(mouse_position_, button);
        if (eventStatus == gui::EventStatus::Consumed) break;
    }
}

void MouseHandler::handleMouseMoved(const sf::Event::MouseMoveEvent& event)
{
    mouse_position_ = sf::Vector2f{static_cast<float>(event.x), static_cast<float>(event.y)};
    for (auto& receiver : receivers_)
    {
        const auto eventStatus = receiver->onMouseMoved(mouse_position_);
        if (eventStatus == gui::EventStatus::Consumed) break;
    }
}

bool MouseHandler::isButtonPressed(sf::Mouse::Button button)
{
    auto iter = buttons_states_.find(button);
    if (iter != buttons_states_.end())
        return iter->second;
    else
    {
        return false;
    }
}
    
}  // namespace engine::input
