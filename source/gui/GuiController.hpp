#pragma once

#include "gui/EventReceiver.hpp"

#include "engine/Logger.hpp"

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

    GuiController(WindowManager& windowManager);
    virtual ~GuiController();


protected:

    Component* hitTest(const gui::event::MousePosition position);

    WindowManager& windowManager_;
    Component* hovered_ = nullptr;
    Component* pressed_ = nullptr;
    engine::Logger logger_;
};

}  // namespace gui
