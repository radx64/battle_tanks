#pragma once

#include "gui/EventReceiver.hpp"

#include "engine/Logger.hpp"

#include "gui/WindowManager.hpp"


namespace gui { class Component; }
namespace gui { class WindowManager; }

namespace gui
{

class GuiController : public gui::EventReceiver
{
public:
    using EventReceiver::receive; // to unshadow EventReceiver not overloaded methods

    EventStatus receive(const event::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus receive(const event::MouseButtonReleased& mouseButtonReleasedEvent) override;
    EventStatus receive(const event::MouseMoved& mouseMovedEvent) override;
    EventStatus receive(const event::FocusChange& focusChangeEvent) override;
    EventStatus receive(const event::KeyboardKeyPressed& keyboardKeyPressedEvent) override;
    EventStatus receive(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent) override;
    EventStatus receive(const event::TextEntered& textEnteredEvent) override;

    GuiController(const sf::Vector2f& mainWindowSize);
    virtual ~GuiController();

    void render(sf::RenderWindow& renderWindow);
    void update();
    MainWindow& mainWindow();
    void openWindow(std::unique_ptr<Window> window);
    void openContextMenu(std::unique_ptr<ContextMenu> menu, const sf::Vector2f& globalPosition);

protected:
    Component* hitTest(const gui::event::MousePosition position);
    Component* getNextFocusableComponent(Component* root, Component* current);
    Component* getPreviousFocusableComponent(Component* root, Component* current);
    void setFocus(Component* component);
    EventStatus updateHover(const gui::event::MousePosition position);
    void onActiveWindowChanged(Window* newActiveWindow);

    WindowManager windowManager_;
    Component* hovered_ = nullptr; //fixme move these nullptr to constructor 
    Component* pressed_ = nullptr;
    Component* focused_ = nullptr;
    engine::Logger logger_;
};

}  // namespace gui
