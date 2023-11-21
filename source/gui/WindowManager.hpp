#ifndef GUI_WINDOW_MANAGER_HPP_
#define GUI_WINDOW_MANAGER_HPP_

#include <list>
#include <memory>

#include <SFML/Graphics.hpp>

#include "gui/EventReceiver.hpp"

namespace gui { class Window; }

namespace gui
{

class WindowManager : public EventReceiver
{
public:
    using EventReceiver::receive;    // to unshadow EventReceiver not overloaded methods

    WindowManager();
    void addWindow(std::unique_ptr<Window> window);
    void render(sf::RenderWindow& renderWindow);
    EventStatus receive(const event::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus receive(const event::MouseButtonReleased& mouseButtonReleasedEvent) override;
    EventStatus receive(const event::MouseMoved& mouseMovedEvent) override;

protected:
    std::list<std::unique_ptr<Window>> windows_;
    Window* active_window_handle_;
};

}  // namespace gui

#endif  // GUI_WINDOW_MANAGER_HPP_