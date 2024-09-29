#pragma once

#include <engine/input/KeyboardReceiver.hpp>

namespace gui {class WindowManager;}

namespace gui
{

class KeyboardController : public engine::input::KeyboardReceiver
{
public:
    KeyboardController(gui::WindowManager* windowManager);

protected:
    void onKeyPressed(const sf::Event::KeyEvent& keyEvent) override;
    void onKeyReleased(const sf::Event::KeyEvent& keyEvent) override;
    
    gui::WindowManager* windowManager_;
};

}  // namespace gui
