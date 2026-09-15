#pragma once

#include <unordered_map>
#include <vector>

#include "engine/events/MouseEvent.hpp"

#include "engine/Clock.hpp"

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
    void handleButtonPressed(const engine::events::MouseButtonPressed& event);
    void handleButtonReleased(const engine::events::MouseButtonReleased& event);
    void handleMouseMoved(const engine::events::MouseMoved& event);
    void handleMouseScrolled(const engine::events::MouseScrolled& event);
    bool isButtonPressed(engine::core::MouseButton button);

protected:
    std::vector<MouseReceiver*> receivers_;
    std::unordered_map<engine::core::MouseButton, bool> buttonsStates_;
    engine::Vec2f mouse_position_;

    Clock::time_point lastLeftClickTimestamp_;
    TimerService* timerService_;
};

    
}  // namespace engine::input
