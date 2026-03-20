#pragma once

#include <list>
#include <memory>

#include <SFML/Graphics.hpp>

#include "engine/TasksQueue.hpp"
#include "engine/Logger.hpp"

#include "gui/EventReceiver.hpp"
#include "gui/Window.hpp"
#include "gui/ContextMenu.hpp"

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

    void openWindow(std::unique_ptr<Window> window);
    void openContextMenu(std::unique_ptr<ContextMenu> menu, const sf::Vector2f& globalPosition);

    void render(sf::RenderWindow& renderWindow);
    //void update();

    EventStatus receive(const event::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus receive(const event::MouseButtonDoublePressed& mouseButtonDoublePressedEvent) override;
    EventStatus receive(const event::MouseButtonReleased& mouseButtonReleasedEvent) override;
    EventStatus receive(const event::MouseMoved& mouseMovedEvent) override;

    EventStatus receive(const event::KeyboardKeyPressed& keyboardKeyPressedEvent) override;
    EventStatus receive(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent) override;
    EventStatus receive(const event::TextEntered& textEntered) override;

    MainWindow& mainWindow();

    Window* getActiveWindow() const;
    void setActiveWindow(Window* window);

    Window* getTopWindowAtPosition(const event::MousePosition position) const;

protected:
    template<class T>
    EventStatus processMouseButton(const T& mouseButtonPressedEvent);

    template<class T>
    EventStatus processEventWithActiveWindow(const T& event);

    void addOverlay(std::unique_ptr<Component> overlay);
    void removeOverlay(Component* overlay);

    std::list<std::unique_ptr<Window>> windows_;
    Window* activeWindow_;
    MainWindow mainWindow_;

    // Components drawn on top of all windows (e.g. context menus)
    std::list<std::unique_ptr<Component>> overlays_;

    sf::RenderTexture renderTexture_;
    sf::Sprite textureSprite_;
    
    engine::TasksQueue tasksQueue_;
    
    Component* getNextFocusableComponent(Component* root, Component* current);
    Component* getPreviousFocusableComponent(Component* root, Component* current);
    engine::Logger logger_;
};

}  // namespace gui
