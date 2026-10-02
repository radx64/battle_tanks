#pragma once

#include "engine/Window.hpp"
#include "engine/gfx/View.hpp"
#include "engine/input/MouseReceiver.hpp"

namespace engine::gui {class EventReceiver;}

namespace engine::gui
{   

class MouseController : public engine::input::MouseReceiver
{
public:
    MouseController(engine::gui::EventReceiver* receiver, engine::Window& window, const engine::gfx::View& view);

protected:
    engine::gui::EventStatus onButtonPressed(const engine::Vec2f& mouse_postion, const engine::core::MouseButton& button, bool double_click) override;
    engine::gui::EventStatus onButtonReleased(const engine::Vec2f& mouse_postion, const engine::core::MouseButton& button) override;
    engine::gui::EventStatus onMouseMoved(const engine::Vec2f& mouse_postion) override;

    engine::gui::EventStatus onMouseScrolled(const engine::Vec2f& mouse_postion, const engine::core::MouseWheel& wheel, const float delta) override;

    engine::Vec2f mapPixelToCoords(const engine::Vec2f& mouse_position);

    engine::gui::EventReceiver* receiver_;
    engine::Window& window_;
    const engine::gfx::View& view_;
};

}  // namespace engine::gui
