#include "engine/input/MouseHandler.hpp"

#include "Config.hpp"

#include "engine/input/MouseReceiver.hpp"
#include "engine/TimerService.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace engine::input
{

MouseHandler::MouseHandler(TimerService* timerService)
: lastLeftClickTimestamp_{}
, timerService_(timerService)
{

}

void MouseHandler::subscribe(MouseReceiver* receiver)
{
    receivers_.push_back(receiver);
    receiver->attach(this);
}

void MouseHandler::unsubscribe(MouseReceiver* receiver)
{
    receivers_.erase(
        std::remove(std::begin(receivers_), std::end(receivers_), receiver),
        std::end(receivers_));
}

void MouseHandler::handleButtonPressed(const sf::Event::MouseButtonEvent& event)
{
    const auto button = event.button;
    buttonsStates_[button] = true;

    bool leftDoubleClicked{false};

    if (button == sf::Mouse::Button::Left)
    {
        auto currentTimestamp = timerService_->getCurrentTime();

        if (currentTimestamp - lastLeftClickTimestamp_ < Config::DOUBLE_CLICK_RATE)
        {
            leftDoubleClicked = true;
        }
        lastLeftClickTimestamp_ = currentTimestamp;
    }

    for (auto* receiver : receivers_)
    {
        // In double click scenario send both double click and single
        // (if double click not consumed) so client can decide what to serve

        auto eventStatus {gui::EventStatus::NotConsumed};

        if (leftDoubleClicked)
        {
            eventStatus = receiver->onButtonPressed(mousePosition_, button, leftDoubleClicked);
        }

        if (eventStatus == gui::EventStatus::NotConsumed)
        {
            constexpr bool singleClicked = false;
            eventStatus = receiver->onButtonPressed(mousePosition_, button, singleClicked);
        }

        if (eventStatus == gui::EventStatus::Consumed) break;
    }
}

void MouseHandler::handleButtonReleased(const sf::Event::MouseButtonEvent& event)
{
    const auto button = event.button;
    buttonsStates_[button] = false;
    for (auto* receiver : receivers_)
    {
        const auto eventStatus = receiver->onButtonReleased(mousePosition_, button);
        if (eventStatus == gui::EventStatus::Consumed) break;
    }
}

void MouseHandler::handleMouseMoved(const sf::Event::MouseMoveEvent& event)
{
    mousePosition_ = sf::Vector2f{static_cast<float>(event.x), static_cast<float>(event.y)};
    for (auto& receiver : receivers_)
    {
        const auto eventStatus = receiver->onMouseMoved(mousePosition_);
        if (eventStatus == gui::EventStatus::Consumed) break;
    }
}

bool MouseHandler::isButtonPressed(sf::Mouse::Button button)
{
    auto iter = buttonsStates_.find(button);
    if (iter != buttonsStates_.end())
        return iter->second;
    else
    {
        return false;
    }
}
    
}  // namespace engine::input
