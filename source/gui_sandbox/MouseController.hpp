#ifndef GUI_SANDBOX_MOUSECONTROLLER_HPP_
#define GUI_SANDBOX_MOUSECONTROLLER_HPP_

#include <SFML/Graphics.hpp>

#include "engine/input/MouseReceiver.hpp"

namespace gui {class WindowManager;}

namespace gui_sandbox
{


// TODO: Rewrite original MouseController to be expandable (eg game uses waypoints)
// To not copy this code everywhere 
class MouseController : public engine::input::MouseReceiver
{
public:
    MouseController(gui::WindowManager* window_manager);

protected:
    void onButtonPressed(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button) override;
    void onButtonReleased(const sf::Vector2f& mouse_postion, const sf::Mouse::Button& button) override;
    void onMouseMoved(const sf::Vector2f& mouse_postion) override;
    gui::WindowManager* window_manager_;

};


}  // namespace gui_sandbox


#endif  // GAME_MOUSECONTROLLER_HPP_