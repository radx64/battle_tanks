#pragma once

#include "engine/core/MouseCode.hpp"
#include "engine/Vector2.hpp"

#include "engine/gui/EventReceiver.hpp"

namespace engine::input {class MouseHandler;}

namespace engine::input
{

class MouseReceiver
{
public:
    MouseReceiver();
    virtual ~MouseReceiver();

    MouseReceiver(const MouseReceiver&) = delete;
    MouseReceiver(const MouseReceiver&&) = delete;
    MouseReceiver& operator=(const MouseReceiver&) = delete;

friend class MouseHandler;

protected:
    virtual engine::gui::EventStatus onButtonPressed(const engine::Vec2f& mouse_position, const engine::core::MouseButton& button, bool double_click) = 0;
    virtual engine::gui::EventStatus onButtonReleased(const engine::Vec2f& mouse_position, const engine::core::MouseButton& button) = 0;
    virtual engine::gui::EventStatus onMouseMoved(const engine::Vec2f& mouse_position) = 0;
    virtual engine::gui::EventStatus onMouseScrolled(const engine::Vec2f& mouse_position, const engine::core::MouseWheel& wheel, const float delta) = 0;

    // attach is used by mousehandler to properly setup destruction of receiver
    void attach(MouseHandler* handler);
    MouseHandler* handler_;
};
    
}  // namespace engine::input
