#pragma once

#include <engine/input/KeyboardReceiver.hpp>

namespace engine::gui {class EventReceiver;}

namespace engine::gui
{

class KeyboardController : public engine::input::KeyboardReceiver
{
public:
    KeyboardController(engine::gui::EventReceiver* receiver);

protected:
    void onKeyPressed(const engine::events::KeyPressedEvent& keyEvent) override;
    void onKeyReleased(const engine::events::KeyReleasedEvent& keyEvent) override;
    
    engine::gui::EventReceiver* receiver_;
};

}  // namespace engine::gui
