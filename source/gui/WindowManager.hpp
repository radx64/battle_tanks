#ifndef GUI_WINDOW_MANAGER_HPP_
#define GUI_WINDOW_MANAGER_HPP_

#include <list>
#include <memory>

#include <SFML/Graphics.hpp>

namespace gui { class Window; }

namespace gui
{

class WindowManager
{
public:
    WindowManager();
    void addWindow(std::unique_ptr<Window> window);
    void render(sf::RenderWindow& renderWindow);
    bool update(const sf::Vector2f& mousePosition, bool isLeftClicked);

protected:
    std::list<std::unique_ptr<Window>> windows_;
    Window* active_window_handle_;
};

}  // namespace gui

#endif  // GUI_WINDOW_MANAGER_HPP_