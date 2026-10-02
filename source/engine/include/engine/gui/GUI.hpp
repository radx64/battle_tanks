#pragma once

#include "engine/gui/EventReceiver.hpp"

#include "engine/Logger.hpp"

#include "engine/gui/WindowManager.hpp"


namespace engine::gui { class Widget; }
namespace engine::gui { class WindowManager; }

namespace engine::gui
{

class GUI : public engine::gui::EventReceiver
{
public:
    using EventReceiver::receive; // to unshadow EventReceiver not overloaded methods

    EventStatus receive(const engine::events::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus receive(const engine::gui::events::MouseButtonDoublePressed& mouseButtonDoublePressedEvent) override;
    EventStatus receive(const engine::events::MouseButtonReleased& mouseButtonReleasedEvent) override;
    EventStatus receive(const engine::events::MouseMoved& mouseMovedEvent) override;
    EventStatus receive(const engine::gui::events::FocusChange& focusChangeEvent) override;
    EventStatus receive(const engine::events::KeyPressed& keyboardKeyPressedEvent) override;
    EventStatus receive(const engine::events::KeyReleased& keyboardKeyReleasedEvent) override;
    EventStatus receive(const engine::events::TextEntered& textEnteredEvent) override;

    GUI(const engine::Vec2f& mainWindowSize);
    virtual ~GUI();

    void render(engine::gfx::RenderTarget& renderTarget);
    void update();
    
    MainWindow& mainWindow();
    void openWindow(std::unique_ptr<Window> window);
    void openContextMenu(std::unique_ptr<ContextMenu> menu, const engine::Vec2f& globalPosition);

    void captureMouse(Widget* widget);
    void releaseMouse();

protected:
    Widget* hitTestWindowsOnly(const engine::Vector2f& position);
    Widget* hitTestOverlaysOnly(const engine::Vector2f& position);
    Widget* hitTest(const engine::Vector2f& position);
    Widget* getNextFocusableWidget(Widget* root, Widget* current);
    Widget* getPreviousFocusableWidget(Widget* root, Widget* current);
    void setFocus(Widget* widget);
    EventStatus updateHover(const engine::Vector2f& position);
    void onActiveWindowChanged(Window* newActiveWindow);
    void onOverlayRemoval(Overlay* removedOverlay); 

    WindowManager windowManager_;
    Widget* hovered_ = nullptr;
    Widget* pressed_ = nullptr;
    Widget* focused_ = nullptr;
    Widget* mouseCapture_ = nullptr;

    engine::Logger logger_;
};

}  // namespace engine::gui
