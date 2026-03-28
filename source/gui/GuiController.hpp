#pragma once

#include "gui/EventReceiver.hpp"

#include "engine/Logger.hpp"

#include "gui/WindowManager.hpp"


namespace gui { class Widget; }
namespace gui { class WindowManager; }

namespace gui
{

class GuiController : public gui::EventReceiver
{
public:
    using EventReceiver::receive; // to unshadow EventReceiver not overloaded methods

    EventStatus receive(const event::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus receive(const event::MouseButtonDoublePressed& mouseButtonDoublePressedEvent) override;
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
    Widget* hitTestWindowsOnly(const gui::event::MousePosition position);
    Widget* hitTestOverlaysOnly(const gui::event::MousePosition position);
    Widget* hitTest(const gui::event::MousePosition position);
    Widget* getNextFocusableWidget(Widget* root, Widget* current);
    Widget* getPreviousFocusableWidget(Widget* root, Widget* current);
    void setFocus(Widget* widget);
    EventStatus updateHover(const gui::event::MousePosition position);
    void onActiveWindowChanged(Window* newActiveWindow);
    void onOverlayRemoval(Overlay* removedOverlay); 

    WindowManager windowManager_;
    Widget* hovered_ = nullptr; //fixme move these nullptr to constructor 
    Widget* pressed_ = nullptr;
    Widget* focused_ = nullptr;
    engine::Logger logger_;
};

}  // namespace gui
