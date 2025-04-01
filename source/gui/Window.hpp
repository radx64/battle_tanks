#pragma once

#include <functional>

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"
#include "gui/FramedSprite.hpp"
#include "gui/StyleSheet.hpp"
#include "gui/window/Config.hpp"

namespace gui { class Label; }

namespace gui::window { class Panel; }
namespace gui::window { class Header; }
namespace gui::window { class StatusBar; }

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
    void onRender(sf::RenderTexture&) override
    {
    }
};

class Window : public Component
{
public:
    Window();

    void setTitle(const std::string_view& text);
    void close();
    bool isDead() const;

    void enable();
    void disable();
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
    bool isInsideHeader(const sf::Vector2f& point);
    bool isInsideResizeGadget(const sf::Vector2f point);

    void onRender(sf::RenderTexture& renderTexture) override;
    void onPositionChange() override;
    void onSizeChange() override;

    EventStatus on(const event::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus on(const event::MouseButtonDoublePressed& mouseButtonDoublePressedEvent) override;
    EventStatus on(const event::MouseButtonReleased& mouseButtonReleasedEvent) override;
    EventStatus on(const event::MouseMoved& mouseMovedEvent) override;

    bool isDead_;
    bool isActive_;
    State state_;
    sf::Vector2f draggingOffset_;

    gui::window::Header* header_;
    gui::window::Panel* windowPanel_;
    gui::window::StatusBar* statusBar_;

    const sf::Texture& activeTexture_;
    const sf::Texture& inactiveTexture_;
    gui::FramedSprite background_;
};

}  // namespace gui
