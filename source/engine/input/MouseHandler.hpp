#pragma once

#include <unordered_map>
#include <vector>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

namespace engine::input { class MouseReceiver; }
namespace engine { class TimerService; }

namespace engine::input
{

// This class acts as a buffer as it only for mouse events whenever
// event is pulled from sfml event queue
// Also it does nice dispatching to receivers
class MouseHandler
{
public:
    MouseHandler(TimerService* timerService);
    void subscribe(MouseReceiver* receiver);
    void unsubscribe(MouseReceiver* receiver);
    void handleButtonPressed(const sf::Event::MouseButtonEvent& event);
    void handleButtonReleased(const sf::Event::MouseButtonEvent& event);
    void handleMouseMoved(const sf::Event::MouseMoveEvent& event);
    bool isButtonPressed(sf::Mouse::Button button);

protected:
    std::vector<MouseReceiver*> receivers_;
    std::unordered_map<sf::Mouse::Button, bool> buttonsStates_;
    sf::Vector2f mousePosition_;

    double lastLeftClickTimestamp_;
    TimerService* timerService_;
};

    
}  // namespace engine::input
