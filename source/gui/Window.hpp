#pragma once

#include <functional>

#include <SFML/Graphics.hpp>

#include "gui/Widget.hpp"
#include "gui/FramedSprite.hpp"
#include "gui/window/Config.hpp"

namespace gui { class Label; }

namespace gui::window { class Panel; }
namespace gui::window { class Header; }
namespace gui::window { class StatusBar; }

namespace gui { class WindowManager; }

namespace gui
{

// Class for main application window layer
// It should always be as a backlayer
// Can be used to place GUI widgets
// on screen
// Can't be moved
// It's size is application window size
class MainWindow : public Widget
{
public:
    using ContextMenuHandler = std::function<void(const sf::Vector2f&)>;

    void setContextMenuHandler(ContextMenuHandler handler);

    void onRender(sf::RenderTexture&) override
    {
    }

protected:
    EventStatus on(const event::MouseButtonPressed& mouseButtonPressedEvent) override;

private:
    ContextMenuHandler contextMenuHandler_;
};

class Window : public Widget
{
public:
    using ContextMenuHandler = std::function<void(const sf::Vector2f&)>;
    using WindowCloseHandler = std::function<void()>;

    Window();

    void setTitle(const std::string_view& text);
    void setContextMenuHandler(ContextMenuHandler handler);
    void setWindowCloseHandler(WindowCloseHandler handler);
    void close();
    bool isDead() const;

    void enable();
    void disable();
    bool isActive() const;

    bool isIdle() const;
    void addChild(std::unique_ptr<Widget> child) override;
    
protected:
    enum class State
    {
        Idle,
        Dragging,
        Resizing
    };
    bool isInState(const Window::State& state) const;
    bool isInsideHeader(const sf::Vector2f& point);
    bool isInsideResizeGadget(const sf::Vector2f point);

    void onRender(sf::RenderTexture& renderTexture) override;
    void onPositionChange() override;
    void onSizeChange() override;
    
    void maximizeRestoreAction();
    
    EventStatus on(const event::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus on(const event::MouseButtonDoublePressed& mouseButtonDoublePressedEvent) override;
    EventStatus on(const event::MouseButtonReleased& mouseButtonReleasedEvent) override;
    EventStatus on(const event::MouseMoved& mouseMovedEvent) override;
    
    void setManager(gui::WindowManager* windowManager);
    friend class gui::WindowManager;

    void setMaximized(const bool state);

    bool isDead_;
    bool isActive_;
    bool isMaximized_;
    State state_;
    sf::Vector2f draggingOffset_;

    gui::window::Header* header_;
    gui::window::Panel* windowPanel_;
    gui::window::StatusBar* statusBar_;

    gui::WindowManager* windowManager_;
    ContextMenuHandler contextMenuHandler_;
    WindowCloseHandler windowCloseHandler_;
    sf::Vector2f windowSizeToRestore_;
    sf::Vector2f windowPositionToRestore_;

    const sf::Texture& activeTexture_;
    const sf::Texture& inactiveTexture_;
    gui::FramedSprite background_;
};

}  // namespace gui
