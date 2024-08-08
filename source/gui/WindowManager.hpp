#pragma once

#include <list>
#include <memory>

#include <SFML/Graphics.hpp>

#include "gui/EventReceiver.hpp"
#include "gui/Window.hpp"

namespace gui { class Window; }
namespace gui { class MainWindow; }

namespace gui
{

class WindowManager : public EventReceiver
{
public:
    using EventReceiver::receive;    // to unshadow EventReceiver not overloaded methods

    WindowManager(const sf::Vector2f& mainWindowSize);
    virtual ~WindowManager();
    
    void addWindow(std::unique_ptr<Window> window);
    void render(sf::RenderWindow& renderWindow);

    EventStatus receive(const event::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus receive(const event::MouseButtonReleased& mouseButtonReleasedEvent) override;
    EventStatus receive(const event::MouseMoved& mouseMovedEvent) override;

    EventStatus receive(const event::KeyboardKeyPressed& keyboardKeyPressedEvent) override;
    EventStatus receive(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent) override;

    MainWindow& mainWindow();

protected:
    std::list<std::unique_ptr<Window>> windows_;
    Window* active_window_handle_;
    MainWindow main_window_;
    sf::RenderTexture render_texture_;
};

}  // namespace gui

