#pragma once

#include <list>
#include <memory>

#include <SFML/Graphics.hpp>

#include "engine/TasksQueue.hpp"
#include "engine/Logger.hpp"

#include "gui/Window.hpp"
#include "gui/ContextMenu.hpp"

namespace gui { class Window; }
namespace gui { class MainWindow; }

namespace gui
{

class WindowManager
{
public:
    using WindowCloseHandler = std::function<void(Window*)>;

    WindowManager(const sf::Vector2f& mainWindowSize);
    virtual ~WindowManager();

    void openWindow(std::unique_ptr<Window> window);
    void openContextMenu(std::unique_ptr<ContextMenu> menu, const sf::Vector2f& globalPosition);

    void render(sf::RenderWindow& renderWindow);
    void update();

    MainWindow& mainWindow();

    Window* getActiveWindow() const;
    std::vector<Window*> getInactiveWindows() const;
    void setActiveWindow(Window* window);
    void setWindowCloseHandler(WindowCloseHandler handler);

    Window* getTopWindowAtPosition(const event::MousePosition position) const;

protected:
    void addOverlay(std::unique_ptr<Component> overlay);
    void removeOverlay(Component* overlay);

    std::list<std::unique_ptr<Window>> windows_;
    Window* activeWindow_;
    MainWindow mainWindow_;

    // Components drawn on top of all windows (e.g. context menus)
    std::list<std::unique_ptr<Component>> overlays_;

    sf::RenderTexture renderTexture_;
    sf::Sprite textureSprite_;

    WindowCloseHandler windowCloseHandler_;
    engine::TasksQueue tasksQueue_;

    engine::Logger logger_;
};

}  // namespace gui
