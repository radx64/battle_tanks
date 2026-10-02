#pragma once

#include <functional>
#include <vector>


#include "engine/gui/ContextMenu.hpp"
#include "engine/gui/Widget.hpp"
#include "engine/gui/FramedSprite.hpp"
#include "engine/gui/window/Config.hpp"

namespace engine::gui { class Label; }

namespace engine::gui::window { class Panel; }
namespace engine::gui::window { class Header; }
namespace engine::gui::window { class StatusBar; }
namespace engine::gui::window { class MenuBar; }

namespace engine::gui { class WindowManager; }

namespace engine::gui
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
    using ContextMenuHandler = std::function<void(const engine::Vec2f&)>;

    void setContextMenuHandler(ContextMenuHandler handler);

    void onRender(engine::gfx::RenderTarget&) override
    {
    }

protected:
    EventStatus on(const engine::events::MouseButtonPressed& mouseButtonPressedEvent) override;

private:
    ContextMenuHandler contextMenuHandler_;
};

class Window : public Widget
{
public:
    using ContextMenuHandler = std::function<void(const engine::Vec2f&)>;
    using WindowCloseHandler = std::function<void()>;

    Window();

    void setTitle(const std::string_view& text);
    void setMenuItems(const std::vector<ContextMenu::Item>& items);
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
    bool isInsideHeader(const engine::Vec2f& point);
    bool isInsideResizeGadget(const engine::Vec2f& point);

    void onRender(engine::gfx::RenderTarget& renderTarget) override;
    void onPositionChange() override;
    void onSizeChange() override;
    
    void maximizeRestoreAction();
    
    EventStatus on(const engine::events::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus on(const engine::gui::events::MouseButtonDoublePressed& mouseButtonDoublePressedEvent) override;
    EventStatus on(const engine::events::MouseButtonReleased& mouseButtonReleasedEvent) override;
    EventStatus on(const engine::events::MouseMoved& mouseMovedEvent) override;
    
    void setManager(engine::gui::WindowManager* windowManager);
    friend class engine::gui::WindowManager;

    void setMaximized(const bool state);
    void prepareWindow();

    bool isDead_;
    bool isActive_;
    bool isMaximized_;
    State state_;
    engine::Vec2f draggingOffset_;

    engine::gui::window::Header* header_;
    engine::gui::window::MenuBar* menuBar_;
    engine::gui::window::Panel* windowPanel_;
    engine::gui::window::StatusBar* statusBar_;

    engine::gui::WindowManager* windowManager_;
    ContextMenuHandler contextMenuHandler_;
    WindowCloseHandler windowCloseHandler_;
    engine::Vec2f windowSizeToRestore_;
    engine::Vec2f windowPositionToRestore_;

    const engine::gfx::Texture& activeTexture_;
    const engine::gfx::Texture& inactiveTexture_;
    engine::gui::FramedSprite background_;
};

}  // namespace engine::gui
