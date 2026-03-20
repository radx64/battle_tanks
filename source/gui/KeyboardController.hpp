#pragma once

#include <engine/input/KeyboardReceiver.hpp>

namespace gui {class EventReceiver;}

namespace gui
{

class KeyboardController : public engine::input::KeyboardReceiver
{
public:
    KeyboardController(gui::EventReceiver* receiver);

protected:
    void onKeyPressed(const sf::Event::KeyEvent& keyEvent) override;
    void onKeyReleased(const sf::Event::KeyEvent& keyEvent) override;
    
    gui::EventReceiver* receiver_;
};

}  // namespace gui
