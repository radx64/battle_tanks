#pragma once

#include <functional>

#include <SFML/Graphics.hpp>

#include "gui/Button.hpp"
#include "gui/Component.hpp"
#include "gui/Label.hpp"
#include "gui/StyleSheet.hpp"
#include "gui/window/Config.hpp"

#include "gui/window/StatusBar.hpp"
#include "gui/window/Header.hpp"

namespace gui { class Label; }
namespace gui { class Panel; }

namespace gui
{

// Class for main application window layer
// It should always be as a backlayer
// Can be used to place GUI components
// on screen
// Can't be moved
// It's size is application window size  
class MainWindow : public Component
{
public:
    void onRender(sf::RenderTexture& renderTexture) override
    {
        UNUSED(renderTexture);
    }
};

class Window : public Component
{
public:
    Window();

    void setTitle(const std::string_view& text);
    void close();
    bool isDead() const;

    void activate();
    void deactivate();
    bool isActive() const;

    bool isIdle() const;
    void addChild(std::unique_ptr<Component> child) override;

protected:
    enum class State
    {
        Idle,
        Dragging,
        Resizing
    };
    bool isInState(const Window::State& state) const;
    bool isInsideHeader(const sf::Vector2f point);
    bool isInsideResizeGadget(const sf::Vector2f point);

    void onRender(sf::RenderTexture& renderTexture) override;
    void onPositionChange() override;
    void onSizeChange() override;

    EventStatus on(const event::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus on(const event::MouseButtonReleased& mouseButtonReleasedEvent) override;
    EventStatus on(const event::MouseMoved& mouseMovedEvent) override;

    bool killed_;
    bool active_;
    State state_;
    sf::Vector2f draggingOffset_;

    gui::window::Header* header_;
    gui::Panel* windowPanel_;
    gui::window::StatusBar* statusBar_;
};

}  // namespace gui
